#ifndef __DEEP_ENGINE_LOGGER_H__
#define __DEEP_ENGINE_LOGGER_H__

#include <DE/def.h>

// ======================= PURE C ========================= //
// | Un simple wrapper qui utilise des pointeurs opaques. | //
// ======================================================== //

typedef struct de_logger_s de_logger_t, *de_hlogger;
  
DE_BEGIN_EXTERN_C

	DE_API de_hlogger de_new_logger(const char *filename);
	DE_API void de_delete_logger(de_hlogger hlogger);
	DE_API void de_logger_add(de_hlogger hlogger, const char *title, const char *text);
	DE_API void de_logger_info(de_hlogger hlogger, const char *text);
	DE_API void de_logger_warn(de_hlogger hlogger, const char *text);
	DE_API void de_logger_error(de_hlogger hlogger, const char *text);
	DE_API void de_logger_success(de_hlogger hlogger, const char *text);
	DE_API void de_logger_callback(de_hlogger hlogger, const char *text);
	DE_API void de_logger_instruction(de_hlogger hlogger, const char *text);
	DE_API void de_logger_append(de_hlogger hlogger, const char *text);

	DE_API int de_logger_open(de_hlogger hlogger);
	DE_API void de_logger_close(de_hlogger hlogger);

DE_END_EXTERN_C

#endif