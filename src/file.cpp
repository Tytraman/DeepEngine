#include <DE/file.hpp>
#include <DE/string_utils.hpp>

#include <stdio.h>

#include <string>

namespace de {

	struct ListWStrFusion {
		List *list;
		const wchar_t *str;
	};


	bool enum_file_print_callback(wchar_t *filename, bool isDirectory, void *args)
	{
		printf("%s %ls\n",
			(isDirectory ? "[]" : " >"),
			filename
		);

		return true;
	}

	bool enum_get_files_callback(wchar_t *filename, bool isDirectory, void *args)
	{
		List *dest = (List *) args;

		if(!isDirectory) {
			wchar_t *copy = StringUtils::copy(filename);
			dest->addCopy(&copy);
		}

		return true;
	}

	bool enum_get_files_ending_callback(wchar_t *filename, bool isDirectory, void *args)
	{
		ListWStrFusion *fusion = (ListWStrFusion *) args;

		if(!isDirectory && StringUtils::endsWith(filename, fusion->str)) {
			wchar_t *copy = StringUtils::copy(filename);
			fusion->list->addCopy(&copy);
		}

		return true;
	}

	/*
	==============
	File::enumFile
	==============
	*/
	bool File::enumFiles(wchar_t *path, FileEnumCallback callback, void *args)
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

		if(handle == INVALID_HANDLE_VALUE) {
			ret = false;
			goto badEnd;
		}

		if(!callback(data.cFileName, data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY, args))
			goto end;

		// Tant qu'un fichier / dossier est trouvé et que la fonction de callback retourne vrai, la boucle tourne.
		while(true) {
			// Permet de trouver le prochain fichier / dossier dans le dossier courant.
			if(!FindNextFileW(handle, &data)) {
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
	File::printAllFiles
	===================
	*/
	void File::printAllFiles(wchar_t *path)
	{
		enumFiles(path, enum_file_print_callback, nullptr);
	}

	/*
	=================
	File::getAllFiles
	=================
	*/
	void File::getAllFiles(wchar_t *path, List *dest)
	{
		enumFiles(path, enum_get_files_callback, dest);
	}

	/*
	===========================
	File::getAllFilesEndingWith
	===========================
	*/
	void File::getAllFilesEndingWith(wchar_t *path, const wchar_t *end, List *dest)
	{
		ListWStrFusion fusion;
		fusion.list = dest;
		fusion.str = end;
		enumFiles(path, enum_get_files_ending_callback, &fusion);
	}

}
