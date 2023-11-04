#include <DE/stream.hpp>
#include <DE/string_utils.hpp>

#include <string.h>

namespace de {

	/*
	========================
	input_stream::input_stream
	========================
	*/
	input_stream::input_stream()
		: m_FD(NULL_FD)
	{ }

	/*
	==========================
	output_stream::output_stream
	==========================
	*/
	output_stream::output_stream()
		: m_FD(NULL_FD)
	{ }

	/*
	==================================
	output_file_stream::output_file_stream
	==================================
	*/
	output_file_stream::output_file_stream(const char *filename)
	{
		size_t length = strlen(filename);

		mem_ptr buffer = mem::alloc(length + 1);
		if(buffer != nullptr) {
			m_Filename = (char *) buffer;
			memcpy(m_Filename, filename, length);
			m_Filename[length] = '\0';
		}else
			m_Filename = nullptr;
	}

	/*
	================================
	input_file_stream::input_file_stream
	================================
	*/
	input_file_stream::input_file_stream(const char *filename)
		: m_WideWchar(false)
	{
		size_t length = strlen(filename);

		mem_ptr buffer = mem::alloc(length + 1);
		if(buffer != nullptr) {
			m_Filename = (char *) buffer;
			memcpy(m_Filename, filename, length);
			m_Filename[length] = '\0';
		}else
			m_Filename = nullptr;
	}

	input_file_stream::input_file_stream(const wchar_t *filename)
		: m_WideWchar(true)
	{
		m_Filename = (char *) string_utils::copy(filename);
	}

	/*
	=====================
	input_file_stream::read
	=====================
	*/
	bool input_file_stream::read(mem_ptr dest, size_t size, size_t *bytesRead)
	{
#if DE_WINDOWS

		DWORD current = 1;

#if DE_32
		if(!ReadFile(m_FD, dest, size, &current, NULL))
			return false;
		*bytesRead = current;

#elif DE_64

		size_t total = 0, diff;
		DWORD number;

		while(total < size && current != 0) {
			diff = size - total;
			number = (diff > 0xFFFFFFFF ? 0xFFFFFFFF : (DWORD) diff);
			
			if(!ReadFile(m_FD, (uint8_t *) dest + total, number, &current, NULL))
				return false;

			total += current;
		}

		*bytesRead = total;

#else
#error Current architecture not supported.
#endif

#elif
#error Need implementation.
#endif

		return true;
	}

	/*
	========================
	input_file_stream::readAll
	========================
	*/
	bool input_file_stream::readAll(mem_ptr dest, size_t *bytesRead)
	{
#if DE_WINDOWS

		DWORD total = 0, current = 1;
		uint8_t buffer[4096];

		while(current != 0) {
			if(!ReadFile(m_FD, buffer, 4096, &current, NULL)) {
				DWORD lastError = GetLastError();
				return false;
			}

			memcpy((uint8_t *) dest + total, buffer, current);

			total += current;
		}

		*bytesRead = total;

#else
#error Need implementation.
#endif

		return true;
	}

	/*
	============================
	input_file_stream::setFilename
	============================
	*/
	void input_file_stream::setFilename(const char *filename)
	{
		size_t length = strlen(filename);

		mem_ptr buffer = mem::realloc(m_Filename, length + 1);
		if(buffer != nullptr) {
			m_Filename = (char *) buffer;
			memcpy(m_Filename, filename, length);
			m_Filename[length] = '\0';
		}
	}

	/*
	=======================
	output_file_stream::write
	=======================
	*/
	bool output_file_stream::write(const mem_ptr buffer, size_t size, size_t *bytesWritten) {
#if DE_WINDOWS
		DWORD current;
#if DE_32
		DWORD current;
		if(!WriteFile(m_FD, buffer, size, &current, NULL))
			return false;
		*bytesWritten = current;
#elif DE_64
		size_t total = 0, diff;
		DWORD number;
		while(total < size) {
			diff = size - total;
			number = (diff > 0xFFFFFFFF ? 0xFFFFFFFF : (DWORD) diff);
			if(!WriteFile(m_FD, (uint8_t *) buffer + total, number, &current, NULL))
				return false;
			total += current;
		}
		*bytesWritten = total;
#else
#error Current architecture not supported.
#endif

#else	// DE_WINDOWS

#error Implement this

#endif

		return true;
	}

	/*
	=============================
	output_file_stream::setFilename
	=============================
	*/
	void output_file_stream::setFilename(const char *filename)
	{
		size_t length = strlen(filename);

		mem_ptr buffer = mem::realloc(m_Filename, length + 1);
		if(buffer != nullptr) {
			m_Filename = (char *) buffer;
			memcpy(m_Filename, filename, length);
			m_Filename[length] = '\0';
		}
	}

}
