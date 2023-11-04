#include <DE/debug.hpp>
#include <DE/sys_win.hpp>
#include <DE/c-wrapper/debug.h>

namespace de
{

    bool debug::addFunctionToCallbackList(const char *functionName, bool count)
    {
	    local_thread_var *local = sys_win::getLocalThreadVar(true);

	    if(local == nullptr)
		    return false;

	    local->functionCallbackList.add(functionName);

	    if(count)
        {
            hash_entry<size_t> *entry = local->functionCallNumber[functionName];
            if(entry != nullptr)
                entry->value++;
	    }

	    return true;
    }

    bool debug::removeFunctionFromCallbackList()
    {
	    local_thread_var *local = sys_win::getLocalThreadVar();

	    if(local == nullptr)
		    return false;

	    local->functionCallbackList.remove(local->functionCallbackList.getNumberOfElements() - 1);
	    return true;
    }

    bool debug::writeToStream(output_stream &stream)
    {
	    size_t i, j;
	    size_t bytesWritten;
	    local_thread_var *local;
	    string line;
        size_t size;

	    local = sys_win::getLocalThreadVar();

	    if(local == nullptr)
		    return false;

        size = local->functionCallbackList.getNumberOfElements();

	    for(i = 0; i < size; ++i)
        {
		    line.clear();

		    for(j = 0; j < i; ++j)
			    line.append("  ");

		    line.append("┗━ ");
            line.append(local->functionCallbackList[i].str());
            line.append("\n");

		    if(!stream.write((mem_ptr) line.str(), line.length(), &bytesWritten) || bytesWritten == 0)
			    return false;
	    }

	    return true;
    }

}


de_bool de_debug_add_function_to_callback_list(const char *name, de_bool count)
{
	return de::debug::addFunctionToCallbackList(name, count);
}

de_bool de_debug_remove_function_from_callback_list()
{
	return de::debug::removeFunctionFromCallbackList();
}

de_bool de_debug_write_to_stream(de_houtput_stream stream)
{
	return de::debug::writeToStream(*reinterpret_cast<de::output_stream *>(stream));
}
