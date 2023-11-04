#include <DE/logger.hpp>
#include <DE/c-wrapper/logger.h>

#include <string>

namespace de {

	logger::logger(const char *filename)
	: output_file_stream(filename)
	{ }

	void append_time(std::string &dest, const char *dateSeparator, const char *timeSeparator, const char *separator)
	{
		char digits[5];
#ifdef DE_WINDOWS
		SYSTEMTIME localTime;
		GetLocalTime(&localTime);
		snprintf(digits, sizeof(digits), "%02d", localTime.wDay);
		dest.append(digits);
		dest.append(dateSeparator);
		snprintf(digits, sizeof(digits), "%02d", localTime.wMonth);
		dest.append(digits);
		dest.append(dateSeparator);
		snprintf(digits, sizeof(digits), "%04d", localTime.wYear);
		dest.append(digits);

		dest.append(separator);

		snprintf(digits, sizeof(digits), "%02d", localTime.wHour);
		dest.append(digits);
		dest.append(timeSeparator);
		snprintf(digits, sizeof(digits), "%02d", localTime.wMinute);
		dest.append(digits);
		dest.append(timeSeparator);
		snprintf(digits, sizeof(digits), "%02d", localTime.wSecond);
		dest.append(digits);
#else
#error Implement this
#endif
	}

	void logger::add(const char *title, const char *text)
	{
		std::string inf = "[";
		size_t bytesWritten;

		append_time(inf, "/", ":", " ");
		inf.append("] [" + std::string(title) + "] " + text);
		write((const mem_ptr) inf.c_str(), inf.length(), &bytesWritten);
	}

}

de_hlogger de_new_logger(const char *filename) {
	return reinterpret_cast<de_hlogger>(new de::logger(filename));
}

void de_delete_logger(de_hlogger hlogger) {
	delete reinterpret_cast<de::logger *>(hlogger);
}

int de_logger_open(de_hlogger hlogger) {
	return reinterpret_cast<de::logger *>(hlogger)->open();
}

void de_logger_close(de_hlogger hlogger) {
	reinterpret_cast<de::logger *>(hlogger)->close();
}

void de_logger_add(de_hlogger hlogger, const char *title, const char *text) {
	reinterpret_cast<de::logger *>(hlogger)->add(title, text);
}

void de_logger_info(de_hlogger hlogger, const char *text) {
	reinterpret_cast<de::logger *>(hlogger)->info(text);
}

void de_logger_warn(de_hlogger hlogger, const char *text) {
	reinterpret_cast<de::logger *>(hlogger)->warn(text);
}
	
void de_logger_error(de_hlogger hlogger, const char *text) {
	reinterpret_cast<de::logger *>(hlogger)->error(text);
}

void de_logger_success(de_hlogger hlogger, const char *text) {
	reinterpret_cast<de::logger *>(hlogger)->success(text);
}

void de_logger_callback(de_hlogger hlogger, const char *text) {
	reinterpret_cast<de::logger *>(hlogger)->callback(text);
}

void de_logger_instruction(de_hlogger hlogger, const char *text) {
	reinterpret_cast<de::logger *>(hlogger)->instruction(text);
}

void de_logger_append(de_hlogger hlogger, const char *text) {
	reinterpret_cast<de::logger *>(hlogger)->append(text);
}
