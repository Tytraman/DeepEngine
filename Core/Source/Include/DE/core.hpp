#ifndef __DEEP_ENGINE_CORE_HPP__
#define __DEEP_ENGINE_CORE_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/error.hpp"
#include "DE/string.hpp"

namespace deep
{

    class core;

    enum class core_init_status : uint8_t
    {
        OK                       = 0,
        InstanceAlreadyExists    = 1,
        NoEnoughDiskSpace        = 2,
        NoEnoughMemory           = 3,
        CannotLoadEngineSettings = 4,
        CannotLoadResources      = 5,
        CannotInitCOM            = 6,
        CannotQueryCpuInfo       = 7,

        Unknown                  = 255
    };

    class DE_API core
    {

        public:
            static core_init_status init(const char *gameTitle, uint64_t diskSpaceRequired, uint64_t physicalRamNeeded, uint64_t virtualRamNeeded);

            static bool focusInstance(const char *gameTitle);
            static bool checkAvailableDiskSpace(uint64_t diskSpaceRequired);
            static bool checkMemory(uint64_t physicalRamNeeded, uint64_t virtualRamNeeded);

            static uint32_t getMousePosition(int *x, int *y);

            static void shutdown();

            static uint64_t getTick();
            static uint64_t getCurrentTimeMillis();
            static void sleep(uint32_t millis);

            static void getLocalTime(uint32_t *year, uint32_t *month, uint32_t *day, uint32_t *hour, uint32_t *minute, uint32_t *second, uint32_t *millis);

            static bool set_pwd(const char *path);
            static string get_pwd();

        private:
            static uint64_t m_InitTime;

        public:
            core() = delete;

    };

    /*
    =============
    core::getTick
    =============
    */
    inline uint64_t core::getTick()
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

}

#endif