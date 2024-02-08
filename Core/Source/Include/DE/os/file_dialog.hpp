#ifndef __DEEP_FILE_DIALOG_HPP__
#define __DEEP_FILE_DIALOG_HPP__

#include "DE/def.hpp"
#include "DE/types.hpp"
#include "DE/window.hpp"
#include "DE/string.hpp"

namespace deep
{

    class open_file_dialog
    {

        public:
            DE_API open_file_dialog(const char *filter = nullptr, uint32_t selectedFilter = 1, bool fileMustExists = true);
            
            DE_API bool show(window *owner = nullptr);
            
            DE_API void set_filters(const char *filter);
            DE_API void set_selected_filter(uint32_t index);

            DE_API string &get_filepath();

        private:
            string m_Filepath;
            string m_Filters;
            uint32_t m_SelectedFilter;
            bool m_FileMustExists;

    };

    /*
    =============================
    open_file_dialog::set_filters
    =============================
    */
    inline void open_file_dialog::set_filters(const char *filter)
    {
        m_Filters = filter;
    }

    /*
    =====================================
    open_file_dialog::set_selected_filter
    =====================================
    */
    inline void open_file_dialog::set_selected_filter(uint32_t index)
    {
        m_SelectedFilter = index;
    }

    /*
    ==============================
    open_file_dialog::get_filepath
    ==============================
    */
    inline string &open_file_dialog::get_filepath()
    {
        return m_Filepath;
    }

}

#endif