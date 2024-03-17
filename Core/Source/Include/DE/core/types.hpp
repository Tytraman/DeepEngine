#ifndef __DEEP_ENGINE_TYPES_HPP__
#define __DEEP_ENGINE_TYPES_HPP__

#include "DE/core/def.hpp"
#include "DE/core/color.hpp"
#include "DE/core/common.hpp"

#include <SDL.h>

/// @namespace de
/// @brief Espace de nom regroupant tout ce qui est relatif au DeepEngine.
namespace deep
{

    // Il n'existe pas dans le standard de version signée de size_t, donc il faut
    // la créer soi-même.
    using ssize_t = make_signed<size_t>;


#if DE_WINDOWS
    typedef WCHAR os_char;

    typedef HANDLE os_fd;
#define os_invalid_fd INVALID_HANDLE_VALUE

#define NULL_FD NULL

#else	// DE_WINDOWS
    typedef char os_char;

    typedef int os_fd;

#define NULL_FD (-1)

#endif	// else DE_WINDOWS

    using de_id = uint64_t;

    constexpr size_t nothing = static_cast<size_t>(-1); ///< Quand rien n'a été trouvé.
    constexpr de_id badID = static_cast<de_id>(-1);     ///< Quand un ID n'est pas valide.
    
    typedef SDL_Keycode keycode;

    namespace direction
    {
        enum Direction : uint8_t
        {
            Left,
            Right,
            Up,
            Down
        };
    }

    DE_UNION(dual8,
        struct u
        {
            uint8_t l;
            uint8_t h;
        };
        uint16_t value;
    );

    DE_UNION(dual16,
        struct u
        {
            uint16_t l;
            uint16_t h;
        };
        uint32_t value;
    );

    DE_UNION(dual32,
        struct u
        {
            uint32_t l;
            uint32_t h;
        };
        uint64_t value;
    );

    struct size
    {
        uint32_t width;
        uint32_t height;

        DE_API size();
        DE_API size(uint32_t width, uint32_t height);
    };

    /*
    ==========
    size::size
    ==========
    */
    inline size::size() 
    {
        width = 0;
        height = 0;
    }

    /*
    ==========
    size::size
    ==========
    */
    inline size::size(uint32_t width, uint32_t height)
    {
        this->width = width;
        this->height = height;
    }

    /// @brief Position X et Y sont formes d'entiers. @ref fposition pour la forme flottante.
    struct position
    {
        int32_t x;
        int32_t y;

        DE_API position();
        DE_API position(int32_t x, int32_t y);
    };

    /*
    ==================
    position::position
    ==================
    */
    inline position::position()
    {
        this->x = 0;
        this->y = 0;
    }

    /*
    ==================
    position::position
    ==================
    */
    inline position::position(int32_t x, int32_t y)
    {
        this->x = x;
        this->y = y;
    }

    /// @brief Position X et Y sont formes de flottants. @ref position pour la forme entière.
    struct fposition
    {
        float x;
        float y;

        DE_API fposition();
        DE_API fposition(float x, float y);
    };

    /*
    ====================
    fposition::fposition
    ====================
    */
    inline fposition::fposition()
    {
        this->x = 0.0f;
        this->y = 0.0f;
    }

    /*
    ====================
    fposition::fposition
    ====================
    */
    inline fposition::fposition(float x, float y)
    {
        this->x = x;
        this->y = y;
    }

    enum class number_format : uint8_t
    {
        Decimal,
        Hexadecimal
    };

    class number_format_option
    {

        public:
            static number_format_option dec();
            static number_format_option hex();

            number_format get_format() const;

        private:
            number_format m_Format;

    };

    inline number_format_option number_format_option::dec()
    {
        number_format_option nfo;
        nfo.m_Format = number_format::Decimal;

        return nfo;
    }

    inline number_format_option number_format_option::hex()
    {
        number_format_option nfo;
        nfo.m_Format = number_format::Hexadecimal;

        return nfo;
    }

    inline number_format number_format_option::get_format() const
    {
        return m_Format;
    }

    static constexpr number_format dec = number_format::Decimal;
    static constexpr number_format hex = number_format::Hexadecimal;

}

#endif