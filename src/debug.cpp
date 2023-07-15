#include <DE/debug.hpp>
#include <DE/sys_win.hpp>
#include <DE/c-wrapper/debug.h>

#include <stdint.h>
#include <algorithm>

bool de::Debug::addFunctionToCallbackList(const char *functionName, bool count) {
	LocalThreadVar *local = SysWin::getLocalThreadVar(true);
	if(local == nullptr)
		return false;

	local->functionCallbackList->emplace_back(functionName);

	if(count) {
		size_t count = (*local->functionCallNumber)[functionName];
		count++;
		(*local->functionCallNumber)[functionName] = count;
	}

	return true;
}

bool de::Debug::removeFunctionFromCallbackList() {
	LocalThreadVar *local = SysWin::getLocalThreadVar();
	if(local == nullptr)
		return false;

	local->functionCallbackList->pop_back();
	return true;
}

bool de::Debug::writeToStream(OutputStream &stream) {
	size_t i, j;
	size_t bytesWritten;
	LocalThreadVar *local;
	std::string line;

	local = SysWin::getLocalThreadVar();
	if(local == nullptr)
		return false;

	for(i = 0; i < local->functionCallbackList->size(); ++i) {
		line.clear();
		for(j = 0; j < i; ++j)
			line.append("  ");
		line.append("┗━ " + (*local->functionCallbackList)[i] + "\n");
		if(!stream.write((uint8_t *) line.c_str(), line.size(), bytesWritten) || bytesWritten == 0)
			return false;
	}

	return true;
}

static bool __compare(const std::pair<std::string, size_t> &pair1, std::pair<std::string, size_t> &pair2) {
	return pair1.second < pair2.second;
}

bool de::Debug::writeCountToStream(OutputStream &stream) {
	size_t bytesWritten;
	LocalThreadVar *local;
	std::string line;

	local = SysWin::getLocalThreadVar();
	if(local == nullptr)
		return false;

	std::vector<std::pair<std::string, size_t>> ordered(local->functionCallNumber->begin(), local->functionCallNumber->end());
	std::sort(ordered.begin(), ordered.end(), __compare);

	for(const std::pair<const std::string, size_t> &it : ordered) {
		line.assign(it.first + " -> " + std::to_string(it.second) + "\n");
		if(!stream.write((uint8_t *) line.c_str(), line.size(), bytesWritten))
			return false;
	}

	return true;
}


de_bool de_debug_add_function_to_callback_list(const char *name, de_bool count) {
	return de::Debug::addFunctionToCallbackList(name, count);
}

de_bool de_debug_remove_function_from_callback_list() {
	return de::Debug::removeFunctionFromCallbackList();
}

de_bool de_debug_write_to_stream(de_houtput_stream stream) {
	return de::Debug::writeToStream(*reinterpret_cast<de::OutputStream *>(stream));
}

de_bool de_debug_write_count_to_stream(de_houtput_stream stream) {
	return de::Debug::writeCountToStream(*reinterpret_cast<de::OutputStream *>(stream));
}
