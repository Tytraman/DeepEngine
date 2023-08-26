#ifndef __DEEP_ENGINE_DEF_H__
#define __DEEP_ENGINE_DEF_H__

#include <math.h>

//===== HELPERs =====//

#define DE_CONCAT_HELPER(__a, __b)          __a##__b
#define DE_STRINGIFY_HELPER(__value)        #__value
#define DE_ENUM_HELPER(__name, __type, ...) enum __name : __type { __VA_ARGS__ }
#define DE_STRUCT_HELPER(__name, ...)                                          \
    struct __name {                                                            \
        __VA_ARGS__                                                            \
    }
#define DE_UNION_HELPER(__name, ...)                                           \
    union __name {                                                             \
        __VA_ARGS__                                                            \
    }

#define DE_CONCAT(__a, __b)   DE_CONCAT_HELPER(__a, __b)
#define DE_STRINGIFY(__value) DE_STRINGIFY_HELPER(__value)

#define DE_ENUM(__name, __type, ...) DE_ENUM_HELPER(__name, __type, __VA_ARGS__)
#define DE_STRUCT(__name, ...)       DE_STRUCT_HELPER(__name, __VA_ARGS__)
#define DE_UNION(__name, ...)        DE_UNION_HELPER(__name, __VA_ARGS__)

#if _WIN32 || __WIN32__
#if _WIN64
#define DE_64 1
#else
#define DE_32 1
#endif

#include <Windows.h>

#define DE_WINDOWS 1

#define DE_DEBUG_BREAK __debugbreak()

#else
// Pour le moment la librairie vise des plateformes sous Windows mais il est prévu
// qu'elle soit disponible pour des systèmes sous Linux.
#error Can't compile on another plateform than Windows at the moment
#endif

// La librairie est écrite en C++ mais est utilisable par des programmes en langage C,
// certaines directives n'existent pas dans les compilateurs C donc il est nécessaire de
// vérifier si les fichiers d'en-têtes sont lues par un compilateur de C ou de C++
#ifdef __cplusplus

#define DE_BEGIN_EXTERN_C extern "C" {
#define DE_END_EXTERN_C }

#else

#define DE_BEGIN_EXTERN_C 
#define DE_END_EXTERN_C 

#endif

#ifdef _MSC_VER
#define DE_MSVC 1
#define DE_FUNCTION_NAME __FUNCTION__
#define DE_FILE_NAME __FILE__
#define DE_LINE __LINE__

#define DE_PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))

#if DEEPENGINE_LIB
#define DE_API __declspec(dllexport)
#else
#define DE_API __declspec(dllimport)
#endif

#elif __GNUC__
#error gcc is not supported
#define DE_GCC 1
#elif __clang__
#error clang is not supported
#define DE_CLANG 1
#endif

#define DE_TRUE  1
#define DE_FALSE 0

#define de DeepEngine

#define DE_DEG_RAD(__deg) (__deg * (float) M_PI / 180.0f)

#endif