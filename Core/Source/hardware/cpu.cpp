#include "DE/hardware/cpu.hpp"
#include "DE/os/COM.hpp"

#include <stdio.h>

namespace deep
{

    CPU *CPU::g_CPU = nullptr;

    /*
    ===============
    CPU::core::core
    ===============
    */
    CPU::logical_processor::logical_processor()
        : L1CacheSize(0),
          L2CacheSize(0),
          L3CacheSize(0),
          associatedCaches(0),
          numberOfL1Caches(0),
          numberOfL2Caches(0),
          numberOfL3Caches(0)
    { }

    /*
    ========
    CPU::CPU
    ========
    */
    CPU::CPU()
        : m_Architecture(CPU::architecture::None),
          m_AddressWidth(0),
          m_NumberOfCores(0),
          m_NumberOfPermormanceCores(0),
          m_NumberOfEfficiencyCores(0),
          m_NumberOfL1Caches(0),
          m_NumberOfL2Caches(0),
          m_NumberOfL3Caches(0)
    { }

    /*
    ===============
    CPU::query_info
    ===============
    */
    bool CPU::query_info()
    {
        COM *com = COM::get_singleton();

        uint16_t architecture;
        if(!com->query_cpu_info(m_Name, &architecture, &m_AddressWidth))
            return false;

        switch(architecture)
        {
            default:
            {
                m_Architecture = architecture::None;
            } break;
            case 0:
            {
                m_Architecture = architecture::x86;
            } break;
            case 1:
            {
                m_Architecture = architecture::MIPS;
            } break;
            case 2:
            {
                m_Architecture = architecture::Alpha;
            } break;
            case 3:
            {
                m_Architecture = architecture::PowerPC;
            } break;
            case 5:
            {
                m_Architecture = architecture::ARM;
            } break;
            case 6:
            {
                m_Architecture = architecture::ia64;
            } break;
            case 9:
            {
                m_Architecture = architecture::x64;
            } break;
            case 12:
            {
                m_Architecture = architecture::ARM64;
            } break;
        }

#if DE_WINDOWS
        DWORD len = 0;
        GetLogicalProcessorInformation(nullptr, &len);

        SYSTEM_LOGICAL_PROCESSOR_INFORMATION *slpi = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION *) mem::alloc(len);
        GetLogicalProcessorInformation(slpi, &len);

        size_t numberOfSlpi = len / sizeof(*slpi);
        size_t i;
        //== const char *level;
        //== const char *bin;
        uint8_t tempNumberOfLogicalProcessors = 0;
        m_NumberOfCores = 0;

        ULONG_PTR mask;
        uint8_t maxNumberOfLogicalProcessors = 0;
        bool firstLogicalProcessorFounded;

        m_LogicalProcessors.empty();
        m_CacheEntries.empty();

        // L'algorithme juste après cette boucle nécessite de connaître le nombre de processeurs logiques.
        // Compte le nombre de processeurs logiques.
        for(i = 0; i < numberOfSlpi; ++i)
        {
#if DE_64
            mask = 0x1000000000000000;
#else
            mask = 0x10000000;
#endif

            firstLogicalProcessorFounded = false;
            for(; mask > 0; mask >>= 1)
            {
                if((slpi[i].ProcessorMask & mask) > 0)
                {
                    if(!firstLogicalProcessorFounded)
                    {
                        firstLogicalProcessorFounded = true;
                        tempNumberOfLogicalProcessors = 0;
                    }
                }

                tempNumberOfLogicalProcessors++;
            }

            if(tempNumberOfLogicalProcessors > maxNumberOfLogicalProcessors)
                maxNumberOfLogicalProcessors = tempNumberOfLogicalProcessors;
        }

        m_LogicalProcessors.reserve(maxNumberOfLogicalProcessors);
        m_LogicalProcessors.fill_with_byte(0);

        // Traitement de chaque élément du tableau de structure.
        for(i = 0; i < numberOfSlpi; ++i)
        {
            switch(slpi[i].Relationship)
            {
                default: break;
                // Quand c'est un attribut de coeur physique.
                case RelationProcessorCore:
                {
                    mask = static_cast<ULONG_PTR>(1) << static_cast<ULONG_PTR>(maxNumberOfLogicalProcessors - 1);
                    tempNumberOfLogicalProcessors = 0;
                    for(; mask > 0; mask >>= 1)
                    {
                        if((slpi[i].ProcessorMask & mask) > 0)
                        {
                            //== bin = DE_TERM_FG_CYAN "1";
                            tempNumberOfLogicalProcessors++;
                        }
                        //== else
                        //==     bin = "0";

                        //== printf("%s" DE_TERM_RESET, bin);
                    }
                    //== printf(" [Core %u]\n", m_NumberOfCores);

                    m_NumberOfCores++;

                    if(tempNumberOfLogicalProcessors > 1)
                        m_NumberOfPermormanceCores++;
                    else
                        m_NumberOfEfficiencyCores++;
                } break;
                // Quand c'est un attribut de cache.
                case RelationCache:
                {
                    cache_level cacheLevel;
                    cache_entry entry;

                    entry.size = slpi[i].Cache.Size;
                    entry.associatedLogicalProcessors = 0;

                    switch(slpi[i].Cache.Level)
                    {
                        default:
                        {
                            //== level = "N/A";
                            cacheLevel = cache_level::None;
                            entry.level = cache_level::None;
                        } break;
                        case 1:
                        {
                            //== level = DE_TERM_FG_GREEN "L1";
                            m_NumberOfL1Caches++;
                            cacheLevel = cache_level::L1;
                            entry.level = cache_level::L1;
                        } break;
                        case 2:
                        {
                            //== level = DE_TERM_FG_YELLOW "L2";
                            m_NumberOfL2Caches++;
                            cacheLevel = cache_level::L2;
                            entry.level = cache_level::L2;
                        } break;
                        case 3:
                        {
                            //== level = DE_TERM_FG_RED "L3";
                            m_NumberOfL3Caches++;
                            cacheLevel = cache_level::L3;
                            entry.level = cache_level::L3;
                        } break;
                    }


                    mask = static_cast<ULONG_PTR>(1) << static_cast<ULONG_PTR>(maxNumberOfLogicalProcessors - 1);
                    firstLogicalProcessorFounded = false;

                    uint8_t bitNumber = maxNumberOfLogicalProcessors;
                    for(; mask > 0; mask >>= 1)
                    {
                        if((slpi[i].ProcessorMask & mask) > 0)
                        {
                            //== bin = DE_TERM_FG_CYAN "1";
                            entry.associatedLogicalProcessors |= mask;

                            logical_processor &proc = m_LogicalProcessors[bitNumber - 1];
                            proc.associatedCaches |= static_cast<uint8_t>(cacheLevel);
                            switch(cacheLevel)
                            {
                                default: break;
                                case cache_level::L1:
                                {
                                    // TODO: se renseigner sur les différents types de caches L1 car il y en a plusieurs.
                                    proc.numberOfL1Caches++;
                                } break;
                                case cache_level::L2:
                                {
                                    proc.numberOfL2Caches++;
                                    proc.L2CacheSize += slpi[i].Cache.Size;
                                } break;
                                case cache_level::L3:
                                {
                                    proc.numberOfL3Caches++;
                                    proc.L3CacheSize += slpi[i].Cache.Size;
                                } break;
                            }
                        }
                        
                        //== else
                        //==     bin = "0";

                        //== printf("%s" DE_TERM_RESET, bin);

                        bitNumber--;
                    }

                    m_CacheEntries.add(entry);
                    //== printf("  [%s" DE_TERM_RESET "] LineSize: %d | Size: %u bytes\n", level, slpi[i].Cache.LineSize, slpi[i].Cache.Size);
                } break;
            }
        }

        //== printf("\n\nNumber of cores: %u", m_NumberOfCores);
        //== if(m_NumberOfEfficiencyCores > 0)
        //==     printf(" (%uP + %uE)", m_NumberOfPermormanceCores, m_NumberOfEfficiencyCores);

        //== printf(
        //==     "\nNumber of L1 caches: %u\n"
        //==     "Number of L2 caches: %u\n"
        //==     "Number of L3 caches: %u\n",
        //==     m_NumberOfL1Caches, m_NumberOfL2Caches, m_NumberOfL3Caches);
#endif

        mem::free(slpi);

        return true;
    }

}
