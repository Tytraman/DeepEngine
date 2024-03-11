#include "DE/core/common.hpp"

namespace deep
{

    /*
    ===============
    next_bit_number
    ===============
    */
    uint64_t next_bit_number(uint64_t value, uint16_t fromBitNumber)
    {
        uint64_t mask = static_cast<uint64_t>(1) << (fromBitNumber - 1);

        for(; mask > 0; mask >>= 1)
        {
            if((value & mask) > 0)
            {
                break;
            }

            fromBitNumber--;
        }

        return fromBitNumber;
    }
    
}
