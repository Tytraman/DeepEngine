#ifndef __DEEP_ENGINE_DEBUG_H__
#define __DEEP_ENGINE_DEBUG_H__

#include <DE/def.h>
#include <DE/c-wrapper/types.h>
#include <DE/c-wrapper/output_stream.h>

DE_BEGIN_EXTERN_C

	DE_API de_bool de_debug_add_function_to_callback_list(const char *name, de_bool count);
	DE_API de_bool de_debug_remove_function_from_callback_list();
	DE_API de_bool de_debug_write_to_stream(de_houtput_stream stream);
	DE_API de_bool de_debug_write_count_to_stream(de_houtput_stream stream);

DE_END_EXTERN_C

#endif