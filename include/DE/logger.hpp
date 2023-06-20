#ifndef __DEEP_ENGINE_LOGGER_HPP__
#define __DEEP_ENGINE_LOGGER_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/output_file_stream.hpp>

#include <string>

namespace de {

	class DE_API Logger : public OutputFileStream {

		public:
			Logger(const char *filename);

			void add(const std::string &title, const std::string &text);
			void info(const std::string &text);
			void warn(const std::string &text);
			void error(const std::string &text);
			void success(const std::string &text);
			void callback(const std::string &text);
			void instruction(const std::string &text);
			void append(const std::string &text);

			static void appendTime(std::string &dest, const char *dateSeparator, const char *timeSeparator, const char *separator);
	};

	inline void Logger::info(const std::string &text) {
		add("INFO", text);
	}

	inline void Logger::warn(const std::string &text) {
		add("WARNING", text);
	}

	inline void Logger::error(const std::string &text) {
		add("ERROR", text);
	}

	inline void Logger::success(const std::string &text) {
		add("SUCCESS", text);
	}

	inline void Logger::callback(const std::string &text) {
		add("CALLBACK", text);
	}

	inline void Logger::instruction(const std::string &text) {
		add("INSTRUCTION", text);
	}

	inline void Logger::append(const std::string &text) {
		OutputFileStream::operator<<(text);
	}

}

#endif