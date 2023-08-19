#ifndef __DEEP_ENGINE_STREAM_HPP__
#define __DEEP_ENGINE_STREAM_HPP__

#include <DE/def.h>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>

#include <stddef.h>
#include <stdint.h>

namespace de {

	class DE_API InputStream {

		protected:
			os_fd m_FD;

			InputStream();

		public:
			virtual bool open() = 0;
			virtual void close() = 0;
			virtual bool read(mem_ptr dest, size_t size, size_t *bytesRead) = 0;
			virtual bool readAll(mem_ptr dest, size_t *bytesRead) = 0;

	};

	class DE_API OutputStream {

		protected:
			os_fd m_FD;

			OutputStream();

		public:
			virtual bool open() = 0;
			virtual bool open(bool append);
			virtual void close() = 0;
			virtual bool write(const mem_ptr buffer, size_t size, size_t *bytesWritten) = 0;
			
	};

	inline bool OutputStream::open(bool append)
	{
		return open();
	}


	class DE_API InputFileStream : public InputStream {

		private:
			char *m_Filename;

		public:
			InputFileStream(const char *filename);
			bool open() override;
			void close() override;
			bool read(mem_ptr dest, size_t size, size_t *bytesRead) override;
			bool readAll(mem_ptr dest, size_t *bytesRead) override;

			const char *getFilename() const;
			size_t getFileSize() const;

			void setFilename(const char *filename);

	};


	inline bool InputFileStream::open()
	{
#if DE_WINDOWS
		m_FD = CreateFileA(m_Filename, FILE_GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(m_FD == INVALID_HANDLE_VALUE)
			return false;
#else
#error Need implementation.
#endif

		return true;
	}

	inline void InputFileStream::close()
	{
#if DE_WINDOWS
		CloseHandle(m_FD);
#else
#error Implement this
#endif
		mem::free(m_Filename);
		m_Filename = nullptr;
		m_FD = NULL_FD;
	}

	inline const char *InputFileStream::getFilename() const
	{
		return m_Filename;
	}

	inline size_t InputFileStream::getFileSize() const
	{
#if DE_WINDOWS
		LARGE_INTEGER largeInteger;

		if(!GetFileSizeEx(m_FD, &largeInteger))
			return 0;

		return largeInteger.QuadPart;
#else
#error Need implementation.
#endif
	}

	class DE_API OutputFileStream : public OutputStream {

		private:
			char *m_Filename;

		public:
			OutputFileStream(const char *filename);
			bool open() override;
			bool open(bool append) override;
			void close() override;
			bool write(const mem_ptr buffer, size_t size, size_t *bytesWritten) override;

			//===== Getters =====
			const char *getFilename() const;

			//===== Setters =====
			void setFilename(const char *filename);
	};

	inline bool OutputFileStream::open()
	{
		return open(false);
	}

	inline bool OutputFileStream::open(bool append)
	{
#ifdef DE_WINDOWS

		if(append) {
			m_FD = CreateFileA(m_Filename, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if(m_FD == INVALID_HANDLE_VALUE)
				return false;
			if(SetFilePointer(m_FD, 0l, nullptr, FILE_END) == INVALID_SET_FILE_POINTER) {
				close();
				return false;
			}
		}else {
			m_FD = CreateFileA(m_Filename, FILE_GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if(m_FD == INVALID_HANDLE_VALUE)
				return false;
		}
		
#else
#error Implement this
#endif
		return true;
	}

	inline void OutputFileStream::close()
	{
#ifdef DE_WINDOWS
		CloseHandle(m_FD);
#else
#error Implement this
#endif
		mem::free(m_Filename);
		m_Filename = nullptr;
		m_FD = NULL_FD;
	}

	inline const char *OutputFileStream::getFilename() const
	{
		return m_Filename;
	}

}

#endif