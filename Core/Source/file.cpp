#include <DE/file.hpp>
#include <DE/string.hpp>
#include <DE/string_utils.hpp>

#include <stdio.h>

#include <string>

namespace deep
{

    struct list_wstr_fusion
    {
        list<wchar_t *> *list;
        const wchar_t *str;
    };


    bool enum_file_print_callback(wchar_t *filename, bool isDirectory, void * /* args */)
    {
        printf("%s %ls\n",
            (isDirectory ? "[]" : " >"),
            filename
        );

        return true;
    }

    bool enum_get_files_callback(wchar_t *filename, bool isDirectory, void *args)
    {
        list<wchar_t *> *dest = static_cast<list<wchar_t *> *>(args);

        if(!isDirectory)
        {
            wchar_t *copy = string_utils::copy(filename);
            dest->add(copy);
        }

        return true;
    }

    bool enum_get_files_ending_callback(wchar_t *filename, bool isDirectory, void *args)
    {
        list_wstr_fusion *fusion = (list_wstr_fusion *) args;

        if(!isDirectory && string_utils::endsWith(filename, fusion->str))
        {
            wchar_t *copy = string_utils::copy(filename);
            fusion->list->add(copy);
        }

        return true;
    }

    /*
    ==============
    file::enumFile
    ==============
    */
    bool file::enumFiles(const char *path, file_enum_callback_ascii callback, void *args)
    {
        // Aucun intérêt de continuer la procédure s'il n'y a pas de fonction callback.
        if(callback == nullptr)
            return true;    // Retourne vrai car il n'y a en soit pas d'erreur.

        #if DE_WINDOWS
        bool ret = true;

        string p(path);

        if(p[p.length() - 1] != L'\\')
            p.append("\\*");
        else if(p[p.length() - 1] != L'*')
            p.append("*");

        WIN32_FIND_DATAA data;

        // Sous Windows on doit d'abord trouver le premier fichier avant de pouvoir lister les autres...
        HANDLE handle = FindFirstFileA(p.str(), &data);

        if(handle == INVALID_HANDLE_VALUE)
        {
            ret = false;
            goto badEnd;
        }

        if(!callback(data.cFileName, data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY, args))
            goto end;

        // Tant qu'un fichier / dossier est trouvé et que la fonction de callback retourne vrai, la boucle tourne.
        while(true)
        {
            // Permet de trouver le prochain fichier / dossier dans le dossier courant.
            if(!FindNextFileA(handle, &data))
            {
                if(GetLastError() == ERROR_NO_MORE_FILES)
                    break;

                ret = false;
                break;
            }

            // Appelle la fonction de callback, si elle retourne faux alors on arrête de chercher la suite des fichiers.
            if(!callback(data.cFileName, data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY, args))
                break;
        }
        

end:
        FindClose(handle);
badEnd:

        return ret;

#else
#error Need implementation
#endif
    }

    /*
    ==============
    file::enumFile
    ==============
    */
    bool file::enumFiles(wchar_t *path, file_enum_callback_wide callback, void *args)
    {
        // Aucun intérêt de continuer la procédure s'il n'y a pas de fonction callback.
        if(callback == nullptr)
            return true;    // Retourne vrai car il n'y a en soit pas d'erreur.

#if DE_WINDOWS
        bool ret = true;

        std::wstring p(path);
        if(p[p.length() - 1] != L'\\')
            p += L"\\*";
        else if(p[p.length() - 1] != L'*')
            p += L'*';

        WIN32_FIND_DATAW data;

        // Sous Windows on doit d'abord trouver le premier fichier avant de pouvoir lister les autres...
        HANDLE handle = FindFirstFileW(p.c_str(), &data);

        if(handle == INVALID_HANDLE_VALUE)
        {
            ret = false;
            goto badEnd;
        }

        if(!callback(data.cFileName, data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY, args))
            goto end;

        // Tant qu'un fichier / dossier est trouvé et que la fonction de callback retourne vrai, la boucle tourne.
        while(true)
        {
            // Permet de trouver le prochain fichier / dossier dans le dossier courant.
            if(!FindNextFileW(handle, &data))
            {
                if(GetLastError() == ERROR_NO_MORE_FILES)
                    break;

                ret = false;
                break;
            }

            // Appelle la fonction de callback, si elle retourne faux alors on arrête de chercher la suite des fichiers.
            if(!callback(data.cFileName, data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY, args))
                break;
        }
        

end:
        FindClose(handle);
badEnd:

        return ret;

#else
#error Need implementation
#endif
    }

    /*
    ===================
    file::printAllFiles
    ===================
    */
    void file::printAllFiles(wchar_t *path)
    {
        enumFiles(path, enum_file_print_callback, nullptr);
    }

    /*
    =================
    file::getAllFiles
    =================
    */
    void file::getAllFiles(wchar_t *path, list<wchar_t *> &dest)
    {
        enumFiles(path, enum_get_files_callback, &dest);
    }

    /*
    ===========================
    file::getAllFilesEndingWith
    ===========================
    */
    void file::getAllFilesEndingWith(wchar_t *path, const wchar_t *end, list<wchar_t *> &dest)
    {
        list_wstr_fusion fusion;
        fusion.list = &dest;
        fusion.str = end;
        enumFiles(path, enum_get_files_ending_callback, &fusion);
    }

}
