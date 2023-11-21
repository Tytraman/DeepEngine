#include <de/file/file_object.hpp>
#include <DE/memory/stack.hpp>

#include <stdio.h>

namespace de
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
            // Si aucun traitement n'est en cours, récupère de nouvelles données.
            if(!processing)
            {
rread:
                // Lit un chunk de données du fichier.
                if(!read(buffer, sizeof(buffer), &bytesRead))
                    return false;

                if(bytesRead == 0)
                    break;

                // Ajoute le chunk lu dans la chaîne de caractères.
                text.append(buffer, bytesRead);
                text.removeAll('\r');

                finder.skipWhiteChars();

                if(finder.position() == text.length())
                    goto rread;
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
                            warningCallback(m_Filename, lines, car);

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

                    elementKey = text.newSubstring(startIndex, endIndex);

                    finder.skip();
                    finder.skipWhiteChars();
                    
                    startIndex = finder.position();

                    if(finder.skipUntil(skipTo, 2));

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

            // Vérifie s'il faut lire de nouveaux octets.
            if(finder.position() >= text.length())
                processing = false;
        }
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
    void file_object_container::enumerate(file_object_enum_element_callback elementCallback, file_object_enum_container_callback containerCallback, mem_ptr args, file_object_container **container, pair<string, string> **element)
    {
        // Permet de garder une trace des conteneurs parents lorsqu'un sous conteneur existe.
        stack<pair<hash_table_iterator<file_object_container>, file_object_container *>> snapshots;
        bool back = false;
        string currentPath = "";

        snapshots.add(pair<hash_table_iterator<file_object_container>, file_object_container *>(this->containers.begin(), this));

        // Tant qu'il reste un conteneur à traiter.
        while(snapshots.count() > 0)
        {
            // Récupère la snapshot précédente contenant la paire itérateur + conteneur actif.
            auto pairIt = snapshots.get();

            // Récupère l'itérateur contenant le prochain conteneur dans lequel énumérer.
            auto &it = pairIt->value1();
            // Récupère le conteneur actif actuellement en train d'être énuméré.
            file_object_container *current = pairIt->value2();
            auto &end = current->containers.end();

            // Si les éléments du conteneur n'ont pas été traités.
            if(!back)
            {
                // Traite d'abord les éléments du conteneur.
                auto elIt  = pairIt->value2()->items.begin();
                auto elEnd = pairIt->value2()->items.end();

                if(elementCallback != nullptr)
                {
                    size_t currentPathLength = currentPath.length();
                    if(currentPathLength > 0)
                    {
                        currentPath.append(".");
                        currentPathLength++;
                    }

                    // Boucle parmis tous les éléments du conteneur.
                    for(; elIt != elEnd; ++elIt)
                    {
                        currentPath.append(elIt->value.value1().str());

                        // Si le callback retourne faux, alors stop l'énumération.
                        if(!elementCallback(*elIt, currentPath, args))
                        {
                            if(element != nullptr)
                                *element = &elIt->value;

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

            back = false;

            auto itBackup = it;

            // Si le conteneur ne possède pas d'autres conteneurs.
            if(it == end)
            {
                // Retire la clé du conteneur du chemin total.
                size_t dotIndex = currentPath.findFromEnd('.');
                if(dotIndex == 0 || dotIndex == static_cast<size_t>(-1))
                    currentPath.clear();
                else
                    currentPath.substring(0, dotIndex);

                back = true;
                snapshots.pop();
            }
            else
            {
                // Ajoute la clé du conteneur dans le chemin total.
                if(currentPath.length() > 0)
                    currentPath.append(".");
                currentPath.append(it->value.name.str());

                // Le conteneur possède d'autres conteneurs.
                // Ajoute une spapshot dans la liste pour y revenir plus tard.
                auto el = it->value.containers.begin();
                snapshots.add(pair<hash_table_iterator<file_object_container>, file_object_container *>(el, &it->value));
                it++;
            }

            if(containerCallback)
            {
                // Si le callback retourne faux, alors stop l'énumération.
                if(!containerCallback(itBackup->value, currentPath, args))
                {
                    if(container != nullptr)
                        *container = &itBackup->value;

                    goto end;
                }
            }
        }
end: ;
    }

    bool __de_file_object_container_search_callback(file_object_container &container, string &currentPath, mem_ptr args)
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

        enumerate(nullptr, __de_file_object_container_search_callback, (mem_ptr) path, &container, nullptr);

        return container;
    }

    bool __de_file_object_element_search_callback(hash_entry<pair<string, string>> &entry, string &currentPath, mem_ptr args)
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

        enumerate(__de_file_object_element_search_callback, nullptr, (mem_ptr) path, nullptr, &element);

        return element;
    }

}
