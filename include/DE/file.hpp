#ifndef __DEEP_ENGINE_FILE_HPP__
#define __DEEP_ENGINE_FILE_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/list.hpp>

namespace de {

	using FileEnumCallback = bool (*)(wchar_t *filename, bool isDirectory, void *args);

	class DE_API File {

		public:
			static bool enumFiles(wchar_t *path, FileEnumCallback callback, void *args);
			static void printAllFiles(wchar_t *path);
			static void getAllFiles(wchar_t *path, List *dest);
			static void getAllFilesEndingWith(wchar_t *path, const wchar_t *end, List *dest);

		private:


	};

}

#endif