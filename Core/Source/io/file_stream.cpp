#include "DE/io/file_stream.hpp"
#include "DE/os/win32.hpp"

namespace deep
{
    
    /*
    ========================
    file_stream::file_stream
    ========================
    */
    file_stream::file_stream(const char *filename, file_mode mode, file_access access, file_share share)
        : m_Filename(filename),
          m_Mode(mode),
          m_Access(access),
          m_Share(share),
          m_FD(os_invalid_fd),
          m_IsOpened(false)
    { }

    /*
    ========================
    file_stream::file_stream
    ========================
    */
    file_stream::file_stream(const char *filename, os_fd fd)
        : m_Filename(filename),
          m_FD(fd),
          m_Mode(file_stream::file_mode::None),
          m_Access(file_stream::file_access::None),
          m_Share(file_stream::file_share::None),
          m_IsOpened(false)
    {
#if DE_WINDOWS

        // Microsoft ne nous laisse pas récupérer nativement les informations d'un handle,
        // donc il faut bien trouver un moyen de le faire...
        // Heureusement c'est possible avec l'API native !

        OBJECT_BASIC_INFORMATION obi;
        ULONG bytes;

        NTSTATUS status = nt_query_object(fd, ObjectBasicInformation, &obi, sizeof(obi), &bytes);
        if(status != DE_NT_STATUS_SUCCESS)
        {
            return;
        }

        bool genericRead = false;
        bool genericWrite = false;

        // GENERIC_READ
        if(
            ((obi.GrantedAccess & FILE_READ_DATA) > 0)       &&
            ((obi.GrantedAccess & FILE_READ_EA) > 0)         &&
            ((obi.GrantedAccess & FILE_READ_ATTRIBUTES) > 0) &&
            ((obi.GrantedAccess & STANDARD_RIGHTS_READ) > 0) &&
            ((obi.GrantedAccess & SYNCHRONIZE) > 0)
        )
        {
            genericRead = true;
        }

        // GENERIC_WRITE
        if(
            ((obi.GrantedAccess & FILE_WRITE_DATA) > 0)       &&
            ((obi.GrantedAccess & FILE_WRITE_ATTRIBUTES) > 0) &&
            ((obi.GrantedAccess & FILE_WRITE_EA) > 0)         &&
            ((obi.GrantedAccess & STANDARD_RIGHTS_WRITE) > 0) &&
            ((obi.GrantedAccess & SYNCHRONIZE) > 0)
        )
        {
            genericWrite = true;
        }

        if(genericRead && genericWrite)
        {
            m_Access = file_access::ReadWrite;
        }
        else if(genericRead)
        {
            m_Access = file_access::Read;
        }
        else if(genericWrite)
        {
            m_Access = file_access::Write;
        }

        m_IsOpened = true;

#else
#error Need implementation
#endif
    }

    /*
    =================
    file_stream::open
    =================
    */
    bool file_stream::open()
    {
        if(m_IsOpened)
        {
            return true;
        }

#if DE_WINDOWS

        DWORD mode = 0;
        DWORD access = 0;
        DWORD share = 0;

        switch(m_Mode)
        {
            default: break;
            case file_mode::Create:
            {
                mode |= CREATE_ALWAYS;
            } break;
            case file_mode::CreateNew:
            {
                mode |= CREATE_NEW;
            } break;
            case file_mode::Append:
            {
                mode |= OPEN_ALWAYS;
                access |= FILE_APPEND_DATA;
            } goto ignore_access;
            case file_mode::Open:
            {
                mode |= OPEN_EXISTING;
            } break;
            case file_mode::OpenOrCreate:
            {
                mode |= OPEN_ALWAYS;
            } break;
            case file_mode::Truncate:
            {
                mode |= TRUNCATE_EXISTING;
            } break;
        }

        switch(m_Access)
        {
            default: break;
            case file_access::Read:
            {
                access |= FILE_GENERIC_READ;
            } break;
            case file_access::Write:
            {
                access |= FILE_GENERIC_WRITE;
            } break;
            case file_access::ReadWrite:
            {
                access |= FILE_GENERIC_READ | FILE_GENERIC_WRITE;
            } break;
        }

ignore_access:

        switch(m_Share)
        {
            default: break;
            case file_share::Read:
            {
                share |= FILE_SHARE_READ;
            } break;
            case file_share::Write:
            {
                share |= FILE_SHARE_WRITE;
            } break;
            case file_share::Delete:
            {
                share |= FILE_SHARE_DELETE;
            } break;
            case file_share::ReadWrite:
            {
                share |= FILE_SHARE_READ | FILE_SHARE_WRITE;
            } break;
        }

        m_FD = CreateFileA(m_Filename.str(), access, share, nullptr, mode, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_POSIX_SEMANTICS, nullptr);

        if(m_FD == os_invalid_fd)
        {
            return false;
        }

        m_IsOpened = true;

        return true;

#else
#error Need implementation
#endif
    }

    /*
    ==================
    file_stream::close
    ==================
    */
    bool file_stream::close()
    {
#if DE_WINDOWS

        if(m_FD == os_invalid_fd)
        {
            return false;
        }

        return CloseHandle(m_FD);

#else
#error Need implementation
#endif
    }

    /*
    =======================
    file_stream::get_length
    =======================
    */
    size_t file_stream::get_length() const
    {
        if(m_FD == os_invalid_fd)
        {
            return 0;
        }

#if DE_WINDOWS

        LARGE_INTEGER linteger;

        if(!GetFileSizeEx(m_FD, &linteger))
        {
            return 0;
        }

        return linteger.QuadPart;

#else
#error Need implementation
#endif
    }

    /*
    =================
    file_stream::read
    =================
    */
    bool file_stream::read(mem_ptr buffer, size_t offset, size_t count, size_t *bytesRead)
    {
#if DE_WINDOWS

        uint8_t *buff = static_cast<uint8_t *>(buffer) + offset;
        size_t total = 0;
        size_t diff = count;

        DWORD bb = 0;

        while(total < count)
        {
            if(!ReadFile(m_FD, buff + total, diff > 4096 ? 4096 : static_cast<DWORD>(diff), &bb, nullptr))
            {
                return false;
            }

            total += bb;

            if(bb == 0)
            {
                break;
            }

            diff = count - total;
        }

        if(bytesRead != nullptr)
        {
            *bytesRead = total;
        }

        return true;

#else
#error Need implementation
#endif
    }

    /*
    ==================
    file_stream::write
    ==================
    */
    bool file_stream::write(mem_ptr buffer, size_t offset, size_t count, size_t *bytesWrite)
    {
#if DE_WINDOWS

        uint8_t *buff = static_cast<uint8_t *>(buffer) + offset;
        size_t total = 0;
        size_t diff = count;

        DWORD bb = 0;

        while(total < count)
        {
            if(!WriteFile(m_FD, buff + total, diff > 4096 ? 4096 : static_cast<DWORD>(diff), &bb, nullptr))
            {
                return false;
            }

            total += bb;

            if(bb == 0)
            {
                break;
            }

            diff = count - total;
        }

        if(bytesWrite != nullptr)
        {
            *bytesWrite = total;
        }

        return true;

#else
#error Need implementation
#endif
    }

    /*
    ====================
    file_stream::copy_to
    ====================
    */
    bool file_stream::copy_to(stream &other)
    {
        size_t position = get_position();
        size_t length = get_length();
        size_t diff;

        // Il ne reste plus rien à lire.
        if(position >= length)
        {
            return true;
        }

        diff = length - position;

        mem_ptr buff = mem::alloc_no_track(diff);
        if(buff == nullptr)
        {
            return false;
        }

        size_t bb = 0;

        if(!read(buff, 0, diff, &bb))
        {
            mem::free_no_track(buff);

            return false;
        }

        return other.write(buff, 0, bb, nullptr) && other.flush();
    }

}
