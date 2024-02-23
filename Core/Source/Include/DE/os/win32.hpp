#ifndef __DEEP_ENGINE_WIN32_HPP__
#define __DEEP_ENGINE_WIN32_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"

namespace deep
{

#if DE_WINDOWS

#pragma comment(lib, "ntdll")

#include <Windows.h>

/*

    Source m'ayant servi pour apprendre à utiliser l'API native de Windows :
    https://www.crow.rip/crows-nest/mal/dev/inject/ntapi-injection/complete-ntapi-implementation

*/

#define DE_NT_STATUS_SUCCESS ((NTSTATUS)0x00000000L)

    typedef struct _UNICODE_STRING
    {
        USHORT Length;
        USHORT MaximumLength;
        PWSTR  Buffer;
    } UNICODE_STRING, *PUNICODE_STRING;

    typedef struct _OBJECT_ATTRIBUTES
    {
        ULONG              Length;
        HANDLE             RootDirectory;
        PUNICODE_STRING    ObjectName;
        ULONG              Attributes;
        PVOID              SecurityDescriptor;
        PVOID              SecurityQualityOfService;
    } OBJECT_ATTRIBUTES, *POBJECT_ATTRIBUTES;

    typedef struct _CLIENT_ID
    {
        PVOID              UniqueProcess;
        PVOID              UniqueThread;
    } CLIENT_ID, *PCLIENT_ID;

    typedef struct _OBJECT_BASIC_INFORMATION
    {
        ULONG Attributes;
        ACCESS_MASK GrantedAccess;
        ULONG HandleCount;
        ULONG PointerCount;
        ULONG PagedPoolCharge;
        ULONG NonPagedPoolCharge;
        ULONG Reserved[3];
        ULONG NameInfoSize;
        ULONG TypeInfoSize;
        ULONG SecurityDescriptorSize;
        LARGE_INTEGER CreationTime;
    } OBJECT_BASIC_INFORMATION, *POBJECT_BASIC_INFORMATION;

    typedef enum _OBJECT_INFORMATION_CLASS
    {
        ObjectBasicInformation, // q: OBJECT_BASIC_INFORMATION
        ObjectNameInformation, // q: OBJECT_NAME_INFORMATION
        ObjectTypeInformation, // q: OBJECT_TYPE_INFORMATION
        ObjectTypesInformation, // q: OBJECT_TYPES_INFORMATION
        ObjectHandleFlagInformation, // qs: OBJECT_HANDLE_FLAG_INFORMATION
        ObjectSessionInformation, // s: void // change object session // (requires SeTcbPrivilege)
        ObjectSessionObjectInformation, // s: void // change object session // (requires SeTcbPrivilege)
        MaxObjectInfoClass
    } OBJECT_INFORMATION_CLASS;

    using NtQueryObject = NTSTATUS (*)(
        _In_opt_ HANDLE Handle,
        _In_ OBJECT_INFORMATION_CLASS ObjectInformationClass,
        _Out_writes_bytes_opt_(ObjectInformationLength) PVOID ObjectInformation,
        _In_ ULONG ObjectInformationLength,
        _Out_opt_ PULONG ReturnLength
    );

    extern NtQueryObject nt_query_object;

#else



#endif

}


#endif