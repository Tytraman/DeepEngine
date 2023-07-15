#include <DE/output_file_stream.hpp>

de::OutputFileStream::OutputFileStream(const std::string &filename)
	: _filename(new std::string(filename))
{}

de::OutputFileStream::~OutputFileStream() {
	delete _filename;
}

bool de::OutputFileStream::write(const uint8_t *buffer, size_t size, size_t &bytesWritten) {
#ifdef DE_WINDOWS

#if DE_32
	DWORD current;
	if(!WriteFile(_fd, buffer, size, &current, NULL))
		return false;
	bytesWritten = current;
#elif DE_64
	size_t total = 0, diff;
	DWORD current, number;
	while(total < size) {
		diff = size - total;
		number = (diff > 0xFFFFFFFF ? 0xFFFFFFFF : (DWORD) diff);
		if(!WriteFile(_fd, buffer + total, number, &current, NULL))
			return false;
		total += current;
	}
#else
#error Current architecture not supported
#endif

#else	// DE_WINDOWS

#error Implement this

#endif

	return true;
}
