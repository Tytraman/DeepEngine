#include <de/file/file_object.hpp>
#include <DE/memory/stack.hpp>

#include <stdio.h>

namespace deep
{

    /*
    ========================
    file_object::file_object
    ========================
    */
    file_object::file_object(const char *filename)
        : input_file_stream(filename),
          m_WarningsNumber(0)
    { }

    /*
    ========================
    file_object::file_object
    ========================
    */
    bool file_object::load(file_object_load_warning_callback warningCallback)
    {
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
            // Si aucun traitement n'est en cours, r�cup�re de nouvelles donn�es.
            if(!processing)
            {
rread:
                // Lit un chunk de donn�es du fichier.
                if(!read(buffer, sizeof(buffer), &bytesRead))
                    return false;

                if(bytesRead == 0)
                    break;

                // Ajoute le chunk lu dans la cha�ne de caract�res.
                text.append(buffer, bytesRead);
                text.removeAll('\r');

                finder.skipWhiteChars();

                if(finder.position() == text.length())
                    goto rread;
            }

            car = finder.current();

            processing = true;

            // Une action se d�clenche selon le caract�re.
            switch(car)
            {
                default:
                {
                    // Si le caract�re actuel n'est pas un caract�re autoris�, alors erreur sur la ligne.
                    if(!finder.isAlphaNum() && car != ' ' && car != '\t')
                    {
                        m_WarningsNumber++;

                        if(warningCallback)
                            warningCallback(m_Filename, lines, car);

                        // Ignore la ligne.
                        finder.skipTo('\n');
                    }

                    // Ignore tous les caract�res d'escpacement car inutiles.
                    finder.skipWhiteChars();

                    // Index du d�but de la cl�.
                    startIndex = finder.position();

                    // Ignore tous les caract�res alphanum�riques et l'underscore.
                    while(
                        finder.skipAlphaNum() ||
                        finder.skip('_'));

                    // Index de fin de la cl�.
                    endIndex = finder.position();

                    finder.skipWhiteChars();
                } break;
                // S�parateur cl� : valeur.
                case ':':
                {
                    char skipTo[2] = { '\n', '}' };

                    elementKey = text.newSubstring(startIndex, endIndex);

                    finder.skip();
                    finder.skipWhiteChars();
                    
                    startIndex = finder.position();

                    finder.skipUntil(skipTo, 2);

                    endIndex = finder.position();

                    string value = text.newSubstring(startIndex, endIndex);

                    current->items.insert(elementKey.str(), pair(elementKey, value));
                } break;
                // Ouverture d'un conteneur.
                case '{':
                {
                    containerKey = text.newSubstring(startIndex, endIndex);
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

            // V�rifie s'il faut lire de nouveaux octets.
            if(finder.position() >= text.length())
                processing = false;
        }

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
    void file_object_container::enumerate(file_object_enum_element_callback elementCallback, file_object_enum_container_callback containerCallback, mem_ptr args, size_t maxSubContainer, file_object_container **container, pair<string, string> **element)
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

                        if(!elementCallback(*itemsIterator, currentPath, args))
                        {
                            if(element != nullptr)
                                *element = &itemsIterator->value;

                            goto end;
                        }

                        if(currentPathLength == 0)
                            currentPath.clear();
                        else
                            currentPath.substring(0, currentPathLength);
                    }

                    if(currentPathLength > 0)
                        currentPath.substring(0, currentPathLength - 1);
                }
            }

            containerProcessed = false;

            hash_table_iterator<file_object_container> lastIteratorBackup = lastIterator;

            if(lastIterator == containerEndIterator)
            {
                // Retire la cl� du conteneur du chemin total.
                size_t dotIndex = currentPath.findFromEnd('.');
                if(dotIndex == 0 || dotIndex == static_cast<size_t>(-1))
                    currentPath.clear();
                else
                    currentPath.substring(0, dotIndex);

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

                // Ajoute la cl� du conteneur dans le chemin total.
                if(currentPath.length() > 0)
                    currentPath.append(".");
                currentPath.append(lastIterator->value.name.str());

                // Le conteneur poss�de d'autres conteneurs.
                // Ajoute une snapshot dans la liste pour y revenir plus tard.
                hash_table_iterator<file_object_container> nextContainersIterator = lastIterator->value.containers.begin();
                snapshots.add(pair<hash_table_iterator<file_object_container>, file_object_container *>(nextContainersIterator, &lastIterator->value));
                lastIterator++;
                indexContainer++;

                if(containerCallback)
                {
                    if(!containerCallback(lastIteratorBackup->value, currentPath, args))
                    {
                        if(container != nullptr)
                            *container = &lastIteratorBackup->value;

                        goto end;
                    }
                }
            }
end_loop: ;
        }
end: ;
    }

    bool __de_file_object_container_search_callback(file_object_container &, string &currentPath, mem_ptr args)
    {
        const char *path = static_cast<const char *>(args);

        return !string_utils::equals(currentPath.str(), path);
    }

    /*
    ======================================
    file_object_container::searchContainer
    ======================================
    */
    file_object_container *file_object_container::searchContainer(const char *path)
    {
        file_object_container *container = nullptr;

        enumerate(nullptr, __de_file_object_container_search_callback, (mem_ptr) path, static_cast<size_t>(-1), &container, nullptr);

        return container;
    }

    bool __de_file_object_element_search_callback(hash_entry<pair<string, string>> & /* entry */, string &currentPath, mem_ptr args)
    {
        const char *path = static_cast<const char *>(args);

        return !string_utils::equals(currentPath.str(), path);
    }

    /*
    ====================================
    file_object_container::searchElement
    ====================================
    */
    pair<string, string> *file_object_container::searchElement(const char *path)
    {
        pair<string, string> *element = nullptr;

        enumerate(__de_file_object_element_search_callback, nullptr, (mem_ptr) path, true, nullptr, &element);

        return element;
    }

}
