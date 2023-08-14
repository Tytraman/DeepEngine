#ifndef __DEEP_ENGINE_STRING_UTILS_HPP__
#define __DEEP_ENGINE_STRING_UTILS_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

namespace de {

	class DE_API StringUtils {

		public:
			static size_t copy(char *dest, size_t destSize, const char *source, size_t sourceLength);

			/// @brief        Copie une cha�ne de caract�res dans un espace m�moire allou� dynamiquement, doit �tre \ref mem:free.
			/// @param source Cha�ne de caract�res � copier.
			/// @return       La cha�ne dynamiquement cr��e ou \c nullptr si une erreur est survenue.
			static char  *copy(const char *source);

			static int toUpper(int value);

	};

}

#endif