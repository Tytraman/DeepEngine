#ifndef __DEEP_ENGINE_STRING_UTILS_HPP__
#define __DEEP_ENGINE_STRING_UTILS_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

namespace de {

	class DE_API StringUtils {

		public:
			static size_t copy(char *dest, size_t destSize, const char *source, size_t sourceLength);

			/// @brief        Copie une chaîne de caractères dans un espace mémoire alloué dynamiquement, doit être \ref mem:free.
			/// @param source Chaîne de caractères à copier.
			/// @return       La chaîne dynamiquement créée ou \c nullptr si une erreur est survenue.
			static char  *copy(const char *source);

			static int toUpper(int value);

	};

}

#endif