#ifndef XAML_UI_SHARED_APPLICATION_HPP
#define XAML_UI_SHARED_APPLICATION_HPP

#include <atomic>
#include <xaml/ui/application.h>

#ifdef XAML_UI_WINDOWS
#include <xaml/ui/win/font_provider.h>
#endif // XAML_UI_WINDOWS

struct xaml_application_impl : xaml_implement<xaml_application_impl, xaml_application, xaml_object>
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
    struct xaml_win32_font_provider_impl : xaml_inner_implement<xaml_win32_font_provider_impl, xaml_application_impl, xaml_win32_font_provider>
    {
        xaml_result XAML_CALL get_default_font(UINT udpi, HFONT* pfont) noexcept override { return m_outer->get_default_font(udpi, pfont); }
    } m_font_provider;

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_win32_font_provider>)
        {
            add_ref();
            *ptr = &m_font_provider;
            return XAML_S_OK;
        }
        else
        {
            return xaml_implement::query(type, ptr);
        }
    }

    xaml_result XAML_CALL get_default_font(UINT, HFONT*) noexcept;
#endif // XAML_UI_WINDOWS
};

#endif // !XAML_UI_SHARED_APPLICATION_HPP
