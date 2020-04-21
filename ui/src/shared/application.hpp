#ifndef XAML_UI_SHARED_APPLICATION_HPP
#define XAML_UI_SHARED_APPLICATION_HPP

#include <atomic>
#include <xaml/ui/application.h>

#ifdef XAML_UI_WINDOWS
#include <xaml/ui/win/font_provider.h>
#endif // XAML_UI_WINDOWS

struct xaml_application_impl
    : xaml_implement<xaml_application_impl, xaml_application, xaml_object
#ifdef XAML_UI_WINDOWS
                     ,
                     xaml_win32_font_provider
#endif // XAML_UI_WINDOWS
                     >
#ifdef XAML_UI_WINDOWS
    ,
      xaml_win32_font_provider
#endif // XAML_UI_WINDOWS
{
private:
    std::atomic<int> m_quit_value{ 0 };
    xaml_ptr<xaml_vector> m_cmd_lines{ nullptr };
    xaml_ptr<xaml_window> m_main_wnd{ nullptr };

public:
    xaml_application_impl(int, xaml_char_t**);

    xaml_result XAML_CALL get_cmd_lines(xaml_vector_view** ptr) noexcept override
    {
        m_cmd_lines.add_ref_to(ptr);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_main_window(xaml_window** ptr) noexcept override
    {
        m_main_wnd.add_ref_to(ptr);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL run(int*) noexcept override;
    xaml_result XAML_CALL quit(int) noexcept override;
    xaml_result XAML_CALL get_theme(xaml_application_theme*) noexcept override;

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL get_default_font(UINT, HFONT*) noexcept override;
#endif // XAML_UI_WINDOWS
};

#endif // !XAML_UI_SHARED_APPLICATION_HPP
