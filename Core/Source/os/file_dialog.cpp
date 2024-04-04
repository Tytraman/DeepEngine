#include "os/file_dialog.hpp"
#include "core/core.hpp"
#include "core/string/string_utils.hpp"

#include <SDL2/SDL_syswm.h>

namespace deep
{

    /*
    ==================================
    open_file_dialog::open_file_dialog
    ==================================
    */
    open_file_dialog::open_file_dialog(const char *filter, uint32_t selectedFilter, bool fileMustExists)
        : m_Filters(filter),
          m_SelectedFilter(selectedFilter),
          m_FileMustExists(fileMustExists)
    { }

    /*
    ======================
    open_file_dialog::show
    ======================
    */
    bool open_file_dialog::show(window *owner)
    {
#if DE_WINDOWS

        string pwd = core::get_pwd();

        char filename[4096];
        filename[0] = '\0';
        OPENFILENAMEA file = { 0 };
        file.lStructSize = sizeof(file);

        if(owner != nullptr)
        {
            SDL_SysWMinfo wmInfo;
            SDL_VERSION(&wmInfo.version);
            SDL_GetWindowWMInfo(owner->get_window(), &wmInfo);

            file.hwndOwner = wmInfo.info.win.window;
        }
        else
        {
            file.hwndOwner = nullptr;
        }

        file.hInstance = 0;

        string filter = m_Filters;
        char *f = rm_const<char *>(filter.str());

        string_utils::replace_all(f, '|', '\0');

        file.lpstrFilter = f;
        file.nFilterIndex = m_SelectedFilter;
        file.lpstrFile = filename;
        file.nMaxFile = sizeof(filename);
        file.lpstrFileTitle = nullptr;
        file.lpstrTitle = nullptr;
        file.Flags = OFN_FORCESHOWHIDDEN;
            
        if(m_FileMustExists)
        {
            file.Flags |= OFN_FILEMUSTEXIST;
        }

        file.nFileOffset = 0;
        file.lpstrDefExt = nullptr;
        file.FlagsEx = 0;

        if(GetOpenFileNameA(&file))
        {
            m_Filepath = file.lpstrFile;

            core::set_pwd(pwd.str());

            return true;
        }

        core::set_pwd(pwd.str());

        return false;

#else
#error Need implementation.
#endif
    }

}
