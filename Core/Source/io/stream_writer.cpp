#include "DE/io/stream_writer.hpp"

namespace deep
{

    /*
    ============================
    stream_writer::stream_writer
    ============================
    */
    stream_writer::stream_writer(stream *stream)
        : m_OutputStream(stream),
          text_writer()
    { }

}
