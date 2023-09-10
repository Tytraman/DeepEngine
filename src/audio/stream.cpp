#include <DE/stream.hpp>
#include <DE/string_utils.hpp>

#include <string.h>

namespace de {

	/*
	========================
	InputStream::InputStream
	========================
	*/
	InputStream::InputStream()
		: m_FD(NULL_FD)
	{ }

	/*
	==========================
	OutputStream::OutputStream
	==========================
	*/
	OutputStream::OutputStream()
		: m_FD(NULL_FD)
	{ }

	/*
	==================================
	OutputFileStream::OutputFileStream
	==================================
	*/
	OutputFileStream::OutputFileStream(const char *filename)
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
	InputFileStream::InputFileStream
	================================
	*/
	InputFileStream::InputFileStream(const char *filename)
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

	InputFileStream::InputFileStream(const wchar_t *filename)
	{
		m_Filename = (char *) StringUtils::copy(filename);
	}

	/*
	=====================
	InputFileStream::read
	=====================
	*/
	bool InputFileStream::read(mem_ptr dest, size_t size, size_t *bytesRead)
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
	InputFileStream::readAll
	========================
	*/
	bool InputFileStream::readAll(mem_ptr dest, size_t *bytesRead)
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
	InputFileStream::setFilename
	============================
	*/
	void InputFileStream::setFilename(const char *filename)
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
	OutputFileStream::write
	=======================
	*/
	bool OutputFileStream::write(const mem_ptr buffer, size_t size, size_t *bytesWritten) {
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
	OutputFileStream::setFilename
	=============================
	*/
	void OutputFileStream::setFilename(const char *filename)
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
