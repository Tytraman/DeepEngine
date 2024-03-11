#ifndef __DEEP_ENGINE_TYPES_H__
#define __DEEP_ENGINE_TYPES_H__

#include "DE/core/def.hpp"

typedef struct de_cpp_string_s de_cpp_string_t, *de_cpp_hstring;

typedef char de_bool;

#if DE_WINDOWS
	typedef __int8  de_sbyte;
	typedef __int16 de_sword;
	typedef __int32 de_sdword;
	typedef __int64 de_sqword;

	typedef unsigned __int8  de_byte;
	typedef unsigned __int16 de_word;
	typedef unsigned __int32 de_dword;
	typedef unsigned __int64 de_qword;

	typedef WCHAR de_os_char;

	typedef HANDLE de_os_fd;
#define NULL_FD NULL

#else


	typedef int os_fd;
#define NULL_FD (-1)
#endif

typedef de_byte  de_uint8;
typedef de_word  de_uint16;
typedef de_dword de_uint32;
typedef de_qword de_uint64;

typedef de_sbyte  de_int8;
typedef de_sword  de_int16;
typedef de_sdword de_int32;
typedef de_sqword de_int64;

#endif