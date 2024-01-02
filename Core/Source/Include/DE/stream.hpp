#ifndef __DEEP_ENGINE_STREAM_HPP__
#define __DEEP_ENGINE_STREAM_HPP__

#include <DE/def.hpp>
#include <DE/types.hpp>
#include <DE/memory/memory.hpp>

#include <stddef.h>
#include <stdint.h>

namespace deep
{

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