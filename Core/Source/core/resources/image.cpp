#include "core/resources/image.hpp"

namespace deep
{

    /*
    ============
    image::image
    ============
    */
    image::image()
        : m_ColorData(nullptr),
          m_Width(0),
          m_Height(0),
          m_ColorModel(color_model::RGB)
    { }

    /*
    ============
    image::image
    ============
    */
    image::image(color_model colorModel)
        : m_ColorData(nullptr),
          m_Width(0),
          m_Height(0),
          m_ColorModel(colorModel)
    { }

    /*
    ==================
    image::copy_column
    ==================
    */
    bool image::copy_column(uint64_t columnToCopy, uint64_t destColumn)
    {
        uint64_t width = m_Width;

        if(columnToCopy == destColumn)
        {
            return true;
        }

        if(columnToCopy >= width || destColumn >= width)
        {
            return false;
        }

        uint8_t bytes = get_components_number();

        uint64_t row;
        size_t rowSize = width * bytes;
        size_t indexOffset;
        size_t destOffset;

        columnToCopy *= bytes;
        destColumn *= bytes;

        for(row = 0; row < width; ++row)
        {
            indexOffset = columnToCopy + rowSize * row;
            destOffset  = destColumn + rowSize * row;

            memcpy(static_cast<uint8_t *>(m_ColorData) + destOffset, static_cast<uint8_t *>(m_ColorData) + indexOffset, bytes);
        }

        return true;
    }

    /*
    ===============
    image::copy_row
    ===============
    */
    bool image::copy_row(const vec2<uint64_t> &fromRowPosition, const vec2<uint64_t> &toRowPosition, uint64_t rowLength, uint64_t fromWidth, uint64_t toWidth, uint8_t bytes, mem_ptr from, mem_ptr to)
    {
        if(toRowPosition.x >= toWidth || toRowPosition.x + rowLength > toWidth)
        {
            return false;
        }

        if(fromRowPosition.x >= fromWidth || fromRowPosition.x + rowLength > fromWidth)
        {
            return false;
        }

        uint64_t rowSize = toWidth * bytes;
        uint64_t fRowSize = fromWidth * bytes;

        uint8_t *dst = static_cast<uint8_t *>(to);
        uint8_t *src = static_cast<uint8_t *>(from);

        memcpy(dst + toRowPosition.y * rowSize + toRowPosition.x * bytes, src + fromRowPosition.y * fRowSize + fromRowPosition.x * bytes, rowLength * bytes);

        return true;
    }

    /*
    =============
    image::resize
    =============
    */
    bool image::resize(uint64_t width, uint64_t height)
    {
        uint64_t currentWidth = m_Width;
        uint64_t currentHeight = m_Height;

        if(currentWidth == width && currentHeight == height)
        {
            return true;
        }

        uint8_t bytes = get_components_number();

        uint8_t *data = static_cast<uint8_t *>(mem::alloc(width * height * bytes));

        if(data == nullptr)
        {
            return false;
        }
        
        uint64_t widthToCopy;
        uint64_t heightToCopy;
        uint64_t leftRowSize;
        uint64_t leftRows;

        uint64_t row;

        if(width < currentWidth)
        {
            widthToCopy = width;

            leftRowSize = 0;
        }
        else
        {
            widthToCopy = currentWidth;

            leftRowSize = width - currentWidth;
        }

        if(height < currentHeight)
        {
            heightToCopy = height;

            leftRows = 0;
        }
        else
        {
            heightToCopy = currentHeight;

            leftRows = height - currentHeight;
        }

        uint64_t rowSize = width * bytes;
        uint64_t trueRowSize = widthToCopy * bytes;
        leftRowSize *= bytes;

        m_Width = width;
        m_Height = height;

        // Copie les lignes.
        for(row = 0; row < heightToCopy; ++row)
        {
            copy_row(vec2<uint64_t>(0, row), vec2<uint64_t>(0, row), widthToCopy, currentWidth, width, bytes, m_ColorData, data);

            memset(data + rowSize * row + trueRowSize, 255, leftRowSize);
        }

        uint64_t index;

        // S'occupe des lignes restantes.
        for(index = 0; index < leftRows; ++index)
        {
            memset(data + rowSize * row, 255, rowSize);

            row++;
        }

        m_ColorData = data;

        return true;
    }

    /*
    ==========
    image::add
    ==========
    */
    bool image::add(ref<image> other, const vec2<uint64_t> &position)
    {
        uint64_t width   = m_Width;
        uint64_t height  = m_Height;
        uint64_t oWidth  = other->get_width();
        uint64_t oHeight = other->get_height();

        uint64_t targetWidth = position.x + oWidth;

        if(targetWidth < width)
        {
            targetWidth = width;
        }

        uint64_t targetHeight = position.y + oHeight;

        if(targetHeight < height)
        {
            targetHeight = height;
        }

        if(!resize(targetWidth, targetHeight))
        {
            return false;
        }

        mem_ptr from = other->get_resource_data();
        mem_ptr to = m_ColorData;

        uint64_t row;

        // Copie chaque ligne de l'autre image dans l'image actuelle.
        for(row = 0; row < oHeight; ++row)
        {
            copy_row(vec2<uint64_t>(0, row), vec2<uint64_t>(position.x, position.y + row), oWidth, other);
        }

        return true;
    }

    /*
    ==========
    image::get
    ==========
    */
    bool image::get(vec3<uint8_t> &dest, uint64_t row, uint64_t column) const
    {
        if(column > m_Width || row > m_Height)
        {
            return false;
        }

        uint64_t index = row * column;
        uint8_t *ptr = static_cast<uint8_t *>(m_ColorData);

        switch(m_ColorModel)
        {
            default: return false;
            case color_model::RGB:
            case color_model::BGR:
            case color_model::RGBA:
            case color_model::BGRA:
            {
                dest.x = ptr[index];
                dest.y = ptr[index + 1];
                dest.z = ptr[index + 2];
            } break;
            case color_model::ARGB:
            {
                dest.x = ptr[index + 1];
                dest.y = ptr[index + 2];
                dest.z = ptr[index + 3];
            } break;
        }

        return true;
    }

    /*
    ==========
    image::get
    ==========
    */
    bool image::get(vec4<uint8_t> &dest, uint64_t row, uint64_t column) const
    {
        if(column > m_Width || row > m_Height)
        {
            return false;
        }

        uint64_t index = row * column;
        uint8_t *ptr = static_cast<uint8_t *>(m_ColorData);

        switch(m_ColorModel)
        {
            default: return false;
            case color_model::RGB:
            case color_model::BGR:
            {
                dest.x = ptr[index];
                dest.y = ptr[index + 1];
                dest.z = ptr[index + 2];
                dest.w = 255;
            } break;
            case color_model::RGBA:
            case color_model::BGRA:
            case color_model::ARGB:
            {
                dest.x = ptr[index];
                dest.y = ptr[index + 1];
                dest.z = ptr[index + 2];
                dest.w = ptr[index + 3];
            } break;
        }

        return true;
    }

}
