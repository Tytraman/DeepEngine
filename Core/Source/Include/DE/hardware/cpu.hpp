#ifndef __DEEP_ENGINE_CPU_HPP__
#define __DEEP_ENGINE_CPU_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/string.hpp"
#include "DE/memory/list.hpp"

namespace deep
{

    class CPU
    {

        public:
            enum class architecture : uint8_t
            {
                None,
                x86,
                MIPS,
                Alpha,
                PowerPC,
                ARM,
                ia64,
                x64,
                ARM64
            };

            enum class cache_level : uint8_t
            {
                None = 0x00,
                L1   = 0x01,
                L2   = 0x02,
                L3   = 0x04
            };

            struct logical_processor
            {
                uint64_t L1CacheSize;
                uint64_t L2CacheSize;
                uint64_t L3CacheSize;
                uint8_t numberOfL1Caches;
                uint8_t numberOfL2Caches;
                uint8_t numberOfL3Caches;
                uint8_t associatedCaches;

                logical_processor();
            };

            struct cache_entry
            {
                cache_level level;
                uint64_t size;
                uint64_t associatedLogicalProcessors;
            };

        public:
            DE_API static CPU *get_singleton();
            DE_API static void set_singleton(CPU *ptr);

            DE_API bool query_info();

            DE_API string get_name() const;
            DE_API list<logical_processor> &get_logical_processors();
            DE_API list<cache_entry> &get_cache_entries();
            DE_API architecture get_architecture() const;
            DE_API const char *get_architecture_str() const;
            DE_API uint8_t get_address_width() const;
            DE_API uint16_t get_number_of_cores() const;
            DE_API uint16_t get_number_of_performance_cores() const;
            DE_API uint16_t get_number_of_efficiency_cores() const;
            DE_API uint16_t get_number_of_L1_caches() const;
            DE_API uint16_t get_number_of_L2_caches() const;
            DE_API uint16_t get_number_of_L3_caches() const;

        private:
            CPU();

        private:
            string m_Name;
            list<logical_processor> m_LogicalProcessors;
            list<cache_entry> m_CacheEntries;
            architecture m_Architecture;
            uint8_t m_AddressWidth;
            uint16_t m_NumberOfCores;
            uint16_t m_NumberOfPermormanceCores;
            uint16_t m_NumberOfEfficiencyCores;
            uint16_t m_NumberOfL1Caches;
            uint16_t m_NumberOfL2Caches;
            uint16_t m_NumberOfL3Caches;

        public:
            friend memory_manager;

        private:
            static CPU *g_CPU;
                
    };

    /*
    ==================
    CPU::get_singleton
    ==================
    */
    inline CPU *CPU::get_singleton()
    {
        return g_CPU;
    }

    /*
    ==================
    CPU::set_singleton
    ==================
    */
    inline void CPU::set_singleton(CPU *ptr)
    {
        g_CPU = ptr;
    }

    /*
    =============
    CPU::get_name
    =============
    */
    inline string CPU::get_name() const
    {
        return m_Name;
    }

    /*
    =====================
    CPU::get_architecture
    =====================
    */
    inline CPU::architecture CPU::get_architecture() const
    {
        return m_Architecture;
    }

    /*
    =========================
    CPU::get_architecture_str
    =========================
    */
    inline const char *CPU::get_architecture_str() const
    {
        switch(m_Architecture)
        {
            default: return "None";
            case architecture::x86: return "x86";
            case architecture::MIPS: return "MIPS";
            case architecture::Alpha: return "Alpha";
            case architecture::PowerPC: return "PowerPC";
            case architecture::ARM: return "ARM";
            case architecture::ia64: return "ia64";
            case architecture::x64: return "x64";
            case architecture::ARM64: return "ARM64";
        }
    }

    /*
    ======================
    CPU::get_address_width
    ======================
    */
    inline uint8_t CPU::get_address_width() const
    {
        return m_AddressWidth;
    }

    /*
    ========================
    CPU::get_number_of_cores
    ========================
    */
    inline uint16_t CPU::get_number_of_cores() const
    {
        return m_NumberOfCores;
    }

    /*
    ====================================
    CPU::get_number_of_performance_cores
    ====================================
    */
    inline uint16_t CPU::get_number_of_performance_cores() const
    {
        return m_NumberOfPermormanceCores;
    }

    /*
    ===================================
    CPU::get_number_of_efficiency_cores
    ===================================
    */
    inline uint16_t CPU::get_number_of_efficiency_cores() const
    {
        return m_NumberOfEfficiencyCores;
    }

    /*
    ===========================
    CPU::get_logical_processors
    ===========================
    */
    inline list<CPU::logical_processor> &CPU::get_logical_processors()
    {
        return m_LogicalProcessors;
    }

    /*
    ======================
    CPU::get_cache_entries
    ======================
    */
    inline list<CPU::cache_entry> &CPU::get_cache_entries()
    {
        return m_CacheEntries;
    }

    /*
    ============================
    CPU::get_number_of_L1_caches
    ============================
    */
    inline uint16_t CPU::get_number_of_L1_caches() const
    {
        return m_NumberOfL1Caches;
    }

    /*
    ============================
    CPU::get_number_of_L2_caches
    ============================
    */
    inline uint16_t CPU::get_number_of_L2_caches() const
    {
        return m_NumberOfL2Caches;
    }

    /*
    ============================
    CPU::get_number_of_L3_caches
    ============================
    */
    inline uint16_t CPU::get_number_of_L3_caches() const
    {
        return m_NumberOfL3Caches;
    }

}

#endif