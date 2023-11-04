#ifndef __DEEP_ENGINE_DEBUG_HPP__
#define __DEEP_ENGINE_DEBUG_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/stream.hpp>

#include <stddef.h>

namespace de
{

	class DE_API debug
    {

		public:
			static bool addFunctionToCallbackList(const char *functionName, bool count = true);
			static bool removeFunctionFromCallbackList();
			static bool writeToStream(output_stream &stream);

        public:
            debug() = delete;

	};

}

#endif