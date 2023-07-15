#ifndef __DEEP_ENGINE_OS_HPP__
#define __DEEP_ENGINE_OS_HPP__

#include <DE/def.h>
#include <DE/types.hpp>

#include <stddef.h>
#include <stdint.h>

namespace de {

	class DE_API OutputStream {

		protected:
			os_fd _fd;

			OutputStream();

		public:
			virtual bool open() = 0;
			virtual void close() = 0;
			virtual bool write(const uint8_t *buffer, size_t size, size_t &bytesWritten) = 0;
			
	};

	inline OutputStream::OutputStream()
		: _fd(NULL_FD) { }

}

#endif