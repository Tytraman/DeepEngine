#ifndef __DEEP_ENGINE_STREAM_HPP__
#define __DEEP_ENGINE_STREAM_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/memory/memory.hpp"
#include "DE/memory/ref_counter.hpp"

#include <stddef.h>
#include <stdint.h>

namespace deep
{

    /// @brief  Interface à la base de toutes les classes de stream.
    /// @remark Très inspirée des Stream sous .NET
    class stream : public reference_counter
    {

        public:
            enum class seek_origin
            {
                Begin,
                Current,
                End
            };

        public:

            virtual bool open() = 0;
            virtual bool close() = 0;
            virtual bool flush() = 0;

            /// @brief  Indique si le stream supporte la lecture.
            /// @return \c true si le stream supporte la lecture.
            virtual bool can_read() const = 0;

            /// @brief  Indique si le stream supporte l'écriture.
            /// @return \c true si le stream supporte l'écriture.
            virtual bool can_write() const = 0;

            /// @brief  Indique si le stream peut timeout.
            /// @return \c true si le stream peut timeout.
            virtual bool can_timeout() const = 0;

            /// @brief  Indique s'il est possible de changer la position du curseur du stream.
            /// @return \c true s'il est possible de changer la position du curseur du stream.
            virtual bool can_seek() const = 0;

            virtual size_t get_length() const = 0;

            virtual size_t get_position() const = 0;

            virtual uint32_t get_read_timeout() const = 0;
            virtual uint32_t get_write_timeout() const = 0;

            virtual void set_read_timeout(uint32_t value) = 0;
            virtual void set_write_timeout(uint32_t value) = 0;

            virtual size_t seek(ssize_t offset, seek_origin origin) = 0;

            /// @brief                Lit \p count octets en les plaçants dans \p buffer à partir d'\p offset.
            /// @remark               Avance le curseur du stream du nombre d'octets lus.
            /// @param[out] buffer    Buffer dans lequel stocker les données à lire.
            /// @param offset         Offset à partir duquel les données seront stockées dans le buffer.
            /// @param count          Nombre d'octets à lire.
            /// @param bytesRead      Pointeur vers une variable qui contiendra le nombre d'octets lus.
            /// @return               \c true si la procédure a réussi.
            virtual bool read(mem_ptr buffer, size_t offset, size_t count, size_t *bytesRead) = 0;

            /// @brief                Écrit \p count octets depuis le \p buffer à partir d'\offset.
            /// @remark               Avance le curseur du stream du nombre d'octets écris.
            /// @param[in] buffer     Buffer depuis lequel écrire les données.
            /// @param offset         Offset à partir duquel les données du buffer seront écrites.
            /// @param count          Nombre d'octets à écrire.
            /// @param bytesWrite     Pointeur vers une variable qui contiendra le nombre d'octets écris.
            /// @return               \c true si la procédure a réussi.
            virtual bool write(mem_ptr buffer, size_t offset, size_t count, size_t *bytesWrite) = 0;
        
            virtual bool copy_to(stream &other) = 0;

            virtual bool is_opened() const = 0;

        protected:
            stream();

    };


    class DE_API input_stream 
    {

        protected:
            os_fd m_FD;

            input_stream();

        public:
            virtual bool open() = 0;
            virtual void close() = 0;
            virtual bool read(mem_ptr dest, size_t size, size_t *bytesRead) = 0;
            virtual bool readAll(mem_ptr dest, size_t *bytesRead) = 0;

    };

    class DE_API output_stream 
    {

        protected:
            os_fd m_FD;

            output_stream();

        public:
            virtual bool open() = 0;
            virtual bool open(bool append, bool recreate);
            virtual void close() = 0;
            virtual bool write(const mem_ptr buffer, size_t size, size_t *bytesWritten) = 0;
            
    };

    /*
    ===================
    output_stream::open
    ===================
    */
    inline bool output_stream::open(bool, bool)
    {
        return open();
    }


    class DE_API input_file_stream : public input_stream 
    {

        public:
            input_file_stream(const char *filename);
            input_file_stream(const wchar_t *filename);

            bool open() override;
            bool openW();

            void close() override;
            bool read(mem_ptr dest, size_t size, size_t *bytesRead) override;
            bool readAll(mem_ptr dest, size_t *bytesRead) override;

            const char *getFilename() const;
            size_t getFileSize() const;

            void setFilename(const char *filename);

            bool isWideChar() const;

        protected:
            char *m_Filename;
            bool m_WideWchar;

    };

    /*
    =======================
    input_file_stream::open
    =======================
    */
    inline bool input_file_stream::open()
    {
#if DE_WINDOWS
        m_FD = CreateFileA(m_Filename, FILE_GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(m_FD == INVALID_HANDLE_VALUE)
            return false;
#else
#error Need implementation.
#endif

        return true;
    }

    /*
    ========================
    input_file_stream::openW
    ========================
    */
    inline bool input_file_stream::openW()
    {
#if DE_WINDOWS
        m_FD = CreateFileW((wchar_t *) m_Filename, FILE_GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if(m_FD == INVALID_HANDLE_VALUE)
            return false;
#else
#error Need implementation.
#endif

        return true;
    }

    /*
    ========================
    input_file_stream::close
    ========================
    */
    inline void input_file_stream::close()
    {
#if DE_WINDOWS
        CloseHandle(m_FD);
#else
#error Implement this
#endif
        mem::free(m_Filename);
        m_Filename = nullptr;
        m_FD = NULL_FD;
    }

    /*
    ==============================
    input_file_stream::getFilename
    ==============================
    */
    inline const char *input_file_stream::getFilename() const
    {
        return m_Filename;
    }

    /*
    ==============================
    input_file_stream::getFileSize
    ==============================
    */
    inline size_t input_file_stream::getFileSize() const
    {
#if DE_WINDOWS
        LARGE_INTEGER largeInteger;

        if(!GetFileSizeEx(m_FD, &largeInteger))
            return 0;

        return largeInteger.QuadPart;
#else
#error Need implementation.
#endif
    }

    /*
    =============================
    input_file_stream::isWideChar
    =============================
    */
    inline bool input_file_stream::isWideChar() const
    {
        return m_WideWchar;
    }

    class DE_API output_file_stream : public output_stream
    {

        private:
            char *m_Filename;

        public:
            output_file_stream(const char *filename);
            bool open() override;
            bool open(bool append, bool recreate) override;
            void close() override;
            bool write(const mem_ptr buffer, size_t size, size_t *bytesWritten) override;

            //===== Getters =====
            const char *getFilename() const;

            //===== Setters =====
            void setFilename(const char *filename);
    };

    /*
    ========================
    output_file_stream::open
    ========================
    */
    inline bool output_file_stream::open()
    {
        return open(false, false);
    }

    /*
    ========================
    output_file_stream::open
    ========================
    */
    inline bool output_file_stream::open(bool append, bool recreate)
    {
#ifdef DE_WINDOWS

        if(append)
        {
            m_FD = CreateFileA(m_Filename, FILE_APPEND_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
            if(m_FD == INVALID_HANDLE_VALUE)
                return false;
            if(SetFilePointer(m_FD, 0l, nullptr, FILE_END) == INVALID_SET_FILE_POINTER) {
                close();
                return false;
            }
        }
        else
        {
            DWORD f;
            if(recreate)
                f = CREATE_ALWAYS;
            else
                f = OPEN_ALWAYS;

            m_FD = CreateFileA(m_Filename, FILE_GENERIC_WRITE, FILE_SHARE_READ, NULL, f, FILE_ATTRIBUTE_NORMAL, NULL);
            if(m_FD == INVALID_HANDLE_VALUE)
                return false;
        }
        
#else
#error Implement this
#endif
        return true;
    }

    /*
    =========================
    output_file_stream::close
    =========================
    */
    inline void output_file_stream::close()
    {
#ifdef DE_WINDOWS
        CloseHandle(m_FD);
#else
#error Implement this
#endif
        mem::free(m_Filename);
        m_Filename = nullptr;
        m_FD = NULL_FD;
    }

    /*
    ===============================
    output_file_stream::getFilename
    ===============================
    */
    inline const char *output_file_stream::getFilename() const
    {
        return m_Filename;
    }

}

#endif