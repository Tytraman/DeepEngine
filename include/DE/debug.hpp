#ifndef __DEEP_ENGINE_DEBUG_HPP__
#define __DEEP_ENGINE_DEBUG_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/output_stream.hpp>

#include <DE/def.h>
#include <stddef.h>
#include <string>
#include <vector>
#include <unordered_map>

namespace de {

	class DE_API Debug {

		private:

		public:
			static bool addFunctionToCallbackList(const char *functionName, bool count = true);
			static bool removeFunctionFromCallbackList();
			static bool writeToStream(OutputStream &stream);
			static bool writeCountToStream(OutputStream &stream);
	};

}

#endif