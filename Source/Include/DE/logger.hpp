#ifndef __DEEP_ENGINE_LOGGER_HPP__
#define __DEEP_ENGINE_LOGGER_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/stream.hpp>

namespace de
{

	class DE_API logger : public output_file_stream
    {

		public:
			logger(const char *filename);

			void add(const char *title, const char *text);
			void info(const char *text);
			void warn(const char *text);
			void error(const char *text);
			void success(const char *text);
			void callback(const char *text);
			void instruction(const char *text);
			void append(const char *text);

	};

	inline void logger::info(const char *text)
    {
		add("INFO", text);
	}

	inline void logger::warn(const char *text)
    {
		add("WARNING", text);
	}

	inline void logger::error(const char *text)
    {
		add("ERROR", text);
	}

	inline void logger::success(const char *text)
    {
		add("SUCCESS", text);
	}

	inline void logger::callback(const char *text)
    {
		add("CALLBACK", text);
	}

	inline void logger::instruction(const char *text)
    {
		add("INSTRUCTION", text);
	}

	inline void logger::append(const char *text)
    {
		add("", text);
	}

}

#endif