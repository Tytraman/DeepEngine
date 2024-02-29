#include "DE/file/file_object.hpp"
#include "DE/memory/stack.hpp"
#include "DE/io/stream_writer.hpp"

#include <stdio.h>

namespace deep
{

    /*
    ========================
    file_object::file_object
    ========================
    */
    file_object::file_object(const char *name)
        : m_Name(name),
          m_WarningsNumber(0)
    { }

    /*
    =================
    file_object::load
    =================
    */
    bool file_object::load(stream *inputStream, file_object_load_warning_callback warningCallback)
    {
        ref<stream> is(inputStream);

        if(!is.is_valid())
        {
            return false;
        }

        if(!is->can_read())
        {
            return false;
        }

        uint8_t buffer[4096];
        size_t bytesRead = 1;

        string text;
        string_finder finder(text);

        stack<file_object_container *> snapshots;
        file_object_container *current = &m_Container;
        snapshots.add(current);

        uint64_t lines = 1;
        size_t startIndex = 0;
        size_t endIndex = 0;

        string elementKey;
        string containerKey;

        bool processing = false;

        char car;

        while(true)
        {
            // Si aucun traitement n'est en cours, récupère de nouvelles données.
            if(!processing)
            {
rread:
                // Lit un chunk de données du fichier.
                if(!is->read(buffer, 0, sizeof(buffer), &bytesRead))
                {
                    return false;
                }

                if(bytesRead == 0)
                {
                    break;
                }

                // Ajoute le chunk lu dans la chaîne de caractères.
                text.append(buffer, bytesRead);
                text.remove_all('\r');

                finder.skipWhiteChars();

                if(finder.position() == text.length())
                {
                    goto rread;
                }
            }

            car = finder.current();

            processing = true;

            // Une action se déclenche selon le caractère.
            switch(car)
            {
                default:
                {
                    // Si le caractère actuel n'est pas un caractère autorisé, alors erreur sur la ligne.
                    if(!finder.isAlphaNum() && car != ' ' && car != '\t')
                    {
                        m_WarningsNumber++;

                        if(warningCallback)
                        {
                            warningCallback(m_Name, lines, car);
                        }

                        // Ignore la ligne.
                        finder.skipTo('\n');
                    }

                    // Ignore tous les caractères d'escpacement car inutiles.
                    finder.skipWhiteChars();

                    // Index du début de la clé.
                    startIndex = finder.position();

                    // Ignore tous les caractères alphanumériques et l'underscore.
                    while(
                        finder.skipAlphaNum() ||
                        finder.skip('_'));

                    // Index de fin de la clé.
                    endIndex = finder.position();

                    finder.skipWhiteChars();
                } break;
                // Séparateur clé : valeur.
                case ':':
                {
                    char skipTo[2] = { '\n', '}' };

                    elementKey = text.new_substring(startIndex, endIndex);

                    finder.skip();
                    finder.skipWhiteChars();
                    
                    startIndex = finder.position();

                    finder.skipUntil(skipTo, 2);

                    endIndex = finder.position();

                    string value = text.new_substring(startIndex, endIndex);

                    current->items.insert(elementKey.str(), pair(elementKey, value));
                } break;
                // Ouverture d'un conteneur.
                case '{':
                {
                    containerKey = text.new_substring(startIndex, endIndex);
                    const char *str = containerKey.str();

                    snapshots.add(current);

                    auto &in = current->containers.insert(str, file_object_container(str));
                    current = &in.value;

                    finder.skip();
                } break;
                // Fermeture d'un conteneur.
                case '}':
                {
                    file_object_container **cont = snapshots.get();
                    current = *cont;
                    snapshots.pop();

                    finder.skip();
                } break;
                case '\n':
                {
                    lines++;
                    finder.skip();
                } break;
            }

            // Vérifie s'il faut lire de nouveaux octets.
            if(finder.position() >= text.length())
            {
                processing = false;
            }
        }

        return true;
    }

    bool __de_file_object_element_save_callback(hash_entry<pair<string, string>> &entry, string &/*currentPath*/, size_t subContainerNumber, mem_ptr args)
    {
        ref<stream_writer> tw(static_cast<stream_writer *>(args));
        size_t i;

        for(i = 0; i < subContainerNumber; ++i)
        {
            *tw << "  ";
        }

        *tw << entry.value.value1().str() << " : " << entry.value.value2().str() << "\n";

        return true;
    }

    bool __de_file_object_new_container_save_callback(file_object_container &container, string &/*currentPath*/, size_t subContainerNumber, mem_ptr args)
    {
        ref<stream_writer> tw(static_cast<stream_writer *>(args));
        size_t i;

        if(subContainerNumber == 1)
        {
            *tw << '\n';
        }

        for(i = 1; i < subContainerNumber; ++i)
        {
            *tw << "  ";
        }

        *tw << container.name.str() << " {\n";

        return true;
    }

    bool __de_file_object_end_container_save_callback(file_object_container &/*container*/, string &/*currentPath*/, size_t subContainerNumber, mem_ptr args)
    {
        ref<stream_writer> tw(static_cast<stream_writer *>(args));
        size_t i;

        for(i = 1; i < subContainerNumber; ++i)
        {
            *tw << "  ";
        }

        *tw << "}\n";

        return true;
    }

    /*
    =================
    file_object::save
    =================
    */
    bool file_object::save(stream *outputStream)
    {
        ref<stream> os(outputStream);

        if(!os.is_valid())
        {
            return false;
        }

        if(!os->can_write())
        {
            return false;
        }

        ref<stream_writer> writer(mem::alloc_type<stream_writer>(os.get()));

        enumerate(__de_file_object_element_save_callback, __de_file_object_new_container_save_callback, __de_file_object_end_container_save_callback, writer.get());

        return true;
    }

    /*
    ============================================
    file_object_container::file_object_container
    ============================================
    */
    file_object_container::file_object_container()
        : items(20),
          containers(20)
    { }

    /*
    ============================================
    file_object_container::file_object_container
    ============================================
    */
    file_object_container::file_object_container(const char *key)
        : name(key),
          items(20),
          containers(20)
    { }

    /*
    ================================
    file_object_container::enumerate
    ================================
    */
    void file_object_container::enumerate(file_object_enum_element_callback elementCallback, file_object_enum_new_container_callback newContainerCallback, file_object_enum_end_container_callback endContainerCallback, mem_ptr args, size_t maxSubContainer, file_object_container **container, pair<string, string> **element)
    {
        stack<pair<hash_table_iterator<file_object_container>, file_object_container *>> snapshots;
        bool containerProcessed = false;
        string currentPath = "";
        size_t indexContainer = 0;

        snapshots.add(pair<hash_table_iterator<file_object_container>, file_object_container *>(this->containers.begin(), this));

        while(snapshots.count() > 0)
        {
            auto lastSnapshot = snapshots.get();
            hash_table_iterator<file_object_container> &lastIterator = lastSnapshot->value1();
            file_object_container *currentContainer = lastSnapshot->value2();
            hash_table_iterator<file_object_container> containerEndIterator = currentContainer->containers.end();

            // S'occupe d'abord des éléments.

            if(!containerProcessed)
            {
                if(elementCallback != nullptr)
                {
                    hash_table_iterator<pair<string, string>> itemsIterator  = currentContainer->items.begin();
                    hash_table_iterator<pair<string, string>> itemsEndIterator = currentContainer->items.end();

                    size_t currentPathLength = currentPath.length();
                    if(currentPathLength > 0)
                    {
                        currentPath.append(".");
                        currentPathLength++;
                    }

                    for(; itemsIterator != itemsEndIterator; ++itemsIterator)
                    {
                        currentPath.append(itemsIterator->value.value1().str());

                        if(!elementCallback(*itemsIterator, currentPath, indexContainer, args))
                        {
                            if(element != nullptr)
                            {
                                *element = &itemsIterator->value;
                            }

                            goto end;
                        }

                        if(currentPathLength == 0)
                        {
                            currentPath.clear();
                        }
                        else
                        {
                            currentPath.substring(0, currentPathLength);
                        }
                    }

                    if(currentPathLength > 0)
                    {
                        currentPath.substring(0, currentPathLength - 1);
                    }
                }
            }

            containerProcessed = false;

            hash_table_iterator<file_object_container> lastIteratorBackup = lastIterator;

            // S'occupe ensuite du conteneur.

            // S'il n'y a pas d'autre conteneur qui suit.
            if(lastIterator == containerEndIterator)
            {
                // Retire la clé du conteneur du chemin total.
                size_t dotIndex = currentPath.find_from_end('.');

                if(endContainerCallback != nullptr && indexContainer > 0)
                {
                    if(!endContainerCallback(*currentContainer, currentPath, indexContainer, args))
                    {
                        goto end;
                    }
                }

                if(dotIndex == 0 || dotIndex == static_cast<size_t>(-1))
                {
                    currentPath.clear();
                }
                else
                {
                    currentPath.substring(0, dotIndex);
                }

                containerProcessed = true;
                snapshots.pop();
                indexContainer--;
            }
            else
            {
                if(indexContainer >= maxSubContainer)
                {
                    lastIterator = containerEndIterator;
                    goto end_loop;
                }

                // Ajoute la clé du conteneur dans le chemin total.
                if(currentPath.length() > 0)
                {
                    currentPath.append(".");
                }
                    
                currentPath.append(lastIterator->value.name.str());

                // Le conteneur possède d'autres conteneurs.
                // Ajoute une snapshot dans la liste pour y revenir plus tard.
                hash_table_iterator<file_object_container> nextContainersIterator = lastIterator->value.containers.begin();
                snapshots.add(pair<hash_table_iterator<file_object_container>, file_object_container *>(nextContainersIterator, &lastIterator->value));
                lastIterator++;
                indexContainer++;

                if(newContainerCallback)
                {
                    if(!newContainerCallback(lastIteratorBackup->value, currentPath, indexContainer, args))
                    {
                        if(container != nullptr)
                        {
                            *container = &lastIteratorBackup->value;
                        }

                        goto end;
                    }
                }
            }
end_loop: ;
        }
end: ;
    }

    bool __de_file_object_container_search_callback(file_object_container &, string &currentPath, size_t /*subContainerNumber*/, mem_ptr args)
    {
        const char *path = static_cast<const char *>(args);

        return !string_utils::equals(currentPath.str(), path);
    }

    /*
    =======================================
    file_object_container::search_container
    =======================================
    */
    file_object_container *file_object_container::search_container(const char *path)
    {
        if(string_utils::is_null_or_whitespace(path))
        {
            return this;
        }

        file_object_container *container = nullptr;

        enumerate(nullptr, __de_file_object_container_search_callback, nullptr, rm_const<char *>(path), static_cast<size_t>(-1), &container, nullptr);

        return container;
    }

    bool __de_file_object_element_search_callback(hash_entry<pair<string, string>> & /* entry */, string &currentPath, size_t /*subContainerNumber*/, mem_ptr args)
    {
        const char *path = static_cast<const char *>(args);

        return !string_utils::equals(currentPath.str(), path);
    }

    /*
    =====================================
    file_object_container::search_element
    =====================================
    */
    pair<string, string> *file_object_container::search_element(const char *path)
    {
        pair<string, string> *element = nullptr;

        enumerate(__de_file_object_element_search_callback, nullptr, nullptr, rm_const<char *>(path), true, nullptr, &element);

        return element;
    }

    /*
    ====================================
    file_object_container::add_container
    ====================================
    */
    file_object_container *file_object_container::add_container(const char *path)
    {
        list<string> conts;
        size_t number = string_utils::split(path, '.', conts);
        size_t i;

        if(number == 0)
        {
            return nullptr;
        }

        hash_table<file_object_container> *current = &containers;
        file_object_container *last = nullptr;

        // Cherche d'abord les conteneurs existants.
        for(i = 0; i < number; ++i)
        {
            hash_entry<file_object_container> *search = (*current)[conts[i].str()];

            if(search == nullptr)
            {
                break;
            }

            current = &search->value.containers;
        }

        for(; i < number; ++i)
        {
            hash_entry<file_object_container> &entry = current->insert(conts[i].str(), file_object_container(conts[i].str()));

            last = &entry.value;
            current = &last->containers;
        }

        return last;
    }

    /*
    ==================================
    file_object_container::add_element
    ==================================
    */
    pair<string, string> *file_object_container::add_element(const char *path, const char *key, const char *value)
    {
        file_object_container *container = search_container(path);

        if(container == nullptr)
        {
            return nullptr;
        }

        return &container->items.insert(key, pair<string, string>(key, value)).value;
    }

}
