#ifndef __DEEP_ENGINE_CORE_HPP__
#define __DEEP_ENGINE_CORE_HPP__

#include "core/def.hpp"
#include "core/types.hpp"
#include "core/string/string.hpp"
#include "core/ref_counted.hpp"
#include "io/text_writer.hpp"

namespace deep
{

    enum class core_init_status
    {
        OK,
        CannotInstantiateObjects,
        InstanceAlreadyExists,
        NoEnoughDiskSpace,
        NoEnoughMemory,
        CannotLoadEngineSettings,
        CannotLoadResources,
        CannotInitCOM,
        CannotQueryCpuInfo,
        CannotInitNtDll,
        CannotLoadStdStream,
        CannotInitResources,

        Unknown
    };

    class core
    {

        public:
            DE_API static core_init_status init(const char *gameTitle, uint64_t diskSpaceRequired, uint64_t physicalRamNeeded, uint64_t virtualRamNeeded);

            DE_API static bool focus_instance(const char *gameTitle);
            DE_API static bool check_available_disk_space(uint64_t diskSpaceRequired);
            DE_API static bool check_memory(uint64_t physicalRamNeeded, uint64_t virtualRamNeeded);

            DE_API static uint32_t get_mouse_position(int *x, int *y);

            DE_API static void shutdown();

            DE_API static uint64_t get_tick();
            DE_API static uint64_t get_current_time_millis();
            DE_API static void sleep(uint32_t millis);

            DE_API static void get_local_time(uint32_t *year, uint32_t *month, uint32_t *day, uint32_t *hour, uint32_t *minute, uint32_t *second, uint32_t *millis);

            DE_API static bool set_pwd(const char *path);
            DE_API static string get_pwd();

            DE_API static void set_out(text_writer *writer);

            DE_API static text_writer &out();
            DE_API static text_writer &err();

        private:
            static uint64_t m_InitTime;
            static ref<text_writer> m_Stderr;
            static ref<text_writer> m_Stdout;

        public:
            core() = delete;

    };

    /*
    ==============
    core::get_tick
    ==============
    */
    inline uint64_t core::get_tick()
    {
#ifdef DE_WINDOWS
        return GetTickCount64() - m_InitTime;
#else
#error Need implementation
#endif
    }

    /*
    ===========
    core::sleep
    ===========
    */
    inline void core::sleep(uint32_t millis)
    {
#ifdef DE_WINDOWS
        Sleep(millis);
#else
#error Need implementation
#endif
    }

    /*
    =========
    core::out
    =========
    */
    inline text_writer &core::out()
    {
        *m_Stdout << dec;

        return *m_Stdout;
    }

    /*
    =========
    core::err
    =========
    */
    inline text_writer &core::err()
    {
        *m_Stderr << dec;

        return *m_Stderr;
    }

}

#endif