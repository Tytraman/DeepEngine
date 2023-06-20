#ifndef __DEEP_ENGINE_OFS_HPP__
#define __DEEP_ENGINE_OFS_HPP__

#include <DE/def.h>
#include <DE/output_stream.hpp>
#include <DE/types.hpp>

#include <string>
#include <stddef.h>

namespace de {

	class DE_API OutputFileStream : public OutputStream {

		private:
			std::string *_filename;

		public:
			OutputFileStream(const std::string &filename);
			~OutputFileStream();
			bool open() override;
			void close() override;
			bool write(const uint8 *buffer, size_t size, size_t &bytesWritten) override;

			template <typename T>
			OutputFileStream &operator<<(const T &value);
			template <typename T>
			OutputFileStream &operator<<(const T *value);

			//===== Getters =====
			std::string &getFilename();

			//===== Setters =====
			void setFilename(const std::string &value);
	};

	inline bool OutputFileStream::open() {
#ifdef DE_WINDOWS
		_fd = CreateFileA(_filename->c_str(), FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if(_fd == INVALID_HANDLE_VALUE)
			return false;
		if(SetFilePointer(_fd, 0l, nullptr, FILE_END) == INVALID_SET_FILE_POINTER) {
			close();
			return false;
		}
#else
#error Implement this
#endif
		return true;
	}

	inline void OutputFileStream::close() {
#ifdef DE_WINDOWS
		CloseHandle(_fd);
#else
#error Implement this
#endif
		_filename->clear();
		_fd = NULL_FD;
	}
	
	template <typename T>
	inline OutputFileStream &OutputFileStream::operator<<(const T &value) {
#ifdef DE_WINDOWS
		DWORD bytesWritten;
		WriteFile(_fd, &value, sizeof(T), &bytesWritten, NULL);
#else
#error Implement this
#endif
		return *this;
	}

	template <typename T>
	inline OutputFileStream &OutputFileStream::operator<<(const T *value) {
		return operator<<(*value);
	}

	template <>
	inline OutputFileStream &OutputFileStream::operator<<<std::string>(const std::string &text) {
#ifdef DE_WINDOWS
		DWORD bytesWritten;
		WriteFile(_fd, text.c_str(), (DWORD) text.length(), &bytesWritten, NULL);
#else
#error Implement this
#endif
		return *this;
	}

	template <>
	inline OutputFileStream &OutputFileStream::operator<<<char>(const char *text) {
#ifdef DE_WINDOWS
	DWORD bytesWritten;
	size_t len = strlen(text);
#if DE_64
	DWORD first = len & 0xFFFFFFFF;
	DWORD second = (len >> 32) & 0xFFFFFFFF;
	WriteFile(_fd, text, first, &bytesWritten, NULL);
	if(second > 0)
		WriteFile(_fd, text + first, second, &bytesWritten, NULL);
#else
	WriteFile(_fd, text, len, &bytesWritten, NULL);
#endif
#else
	#error Implement this
#endif
	return *this;
}

	inline std::string &OutputFileStream::getFilename() {
		return *_filename;
	}

	inline void OutputFileStream::setFilename(const std::string &value) {
		_filename->assign(value);
	}


}


#endif