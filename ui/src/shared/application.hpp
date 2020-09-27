#ifndef XAML_UI_SHARED_APPLICATION_HPP
#define XAML_UI_SHARED_APPLICATION_HPP

#include <atomic>
#include <xaml/event.h>
#include <xaml/ui/application.h>

#ifdef XAML_UI_WINDOWS
    #include <xaml/ui/win/font_provider.h>
#elif defined(XAML_UI_GTK3)
    #include <gtk/gtk.h>
    #include <gtk3/resources.hpp>
    #include <xaml/ui/gtk3/application.h>
#elif defined(XAML_UI_QT5)
    #include <QApplication>
    #include <memory>
#endif

struct xaml_application_impl : xaml_implement<xaml_application_impl, xaml_application>
{
protected:
#ifdef XAML_UI_GTK3
    g_object_unique_ptr<GtkApplication> m_native_app{};
#elif defined(XAML_UI_QT5)
    int m_argc{};
    std::unique_ptr<QApplication> m_native_app{};
#endif

    std::atomic<int> m_quit_value{ 0 };
    xaml_ptr<xaml_vector<xaml_string>> m_cmd_lines{ nullptr };
    xaml_ptr<xaml_window> m_main_wnd{ nullptr };

public:
    ~xaml_application_impl() override;

    xaml_result XAML_CALL init(int, char**) noexcept;

    xaml_result XAML_CALL get_cmd_lines(xaml_vector_view<xaml_string>** ptr) noexcept override
    {
        return m_cmd_lines->query(ptr);
    }

    xaml_result XAML_CALL get_main_window(xaml_window** ptr) noexcept override
    {
        return m_main_wnd.query(ptr);
    }

    xaml_result XAML_CALL run(int*) noexcept override;
    xaml_result XAML_CALL quit(int) noexcept override;
    xaml_result XAML_CALL get_theme(xaml_application_theme*) noexcept override;

    xaml_result XAML_CALL window_added(xaml_window* window) noexcept override
    {
        if (!m_main_wnd) m_main_wnd = window;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL window_removed(xaml_window* window) noexcept override
    {
        if (m_main_wnd.get() == window)
        {
            m_main_wnd = nullptr;
            return quit(0);
        }
        return XAML_S_OK;
    }

    XAML_EVENT_IMPL(activate, xaml_object, xaml_application)

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
#elif defined(XAML_UI_GTK3)
    static void on_activate_event(GApplication*, xaml_application_impl*) noexcept;

    xaml_result XAML_CALL get_handle(GtkApplication** ptr) noexcept
    {
        *ptr = m_native_app.get();
        return XAML_S_OK;
    }
    xaml_result XAML_CALL set_handle(GtkApplication* value) noexcept
    {
        m_native_app.reset(value);
        return XAML_S_OK;
    }

    struct xaml_gtk3_application_impl : xaml_inner_implement<xaml_gtk3_application_impl, xaml_application_impl, xaml_gtk3_application>
    {
        xaml_result XAML_CALL get_handle(GtkApplication** ptr) noexcept override { return m_outer->get_handle(ptr); }
        xaml_result XAML_CALL set_handle(GtkApplication* value) noexcept override { return m_outer->set_handle(value); }
    } m_native_impl;

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_gtk3_application>)
        {
            add_ref();
            *ptr = &m_native_impl;
            return XAML_S_OK;
        }
        else
        {
            return xaml_implement::query(type, ptr);
        }
    }
#endif
};

#endif // !XAML_UI_SHARED_APPLICATION_HPP
