#ifndef XAML_UI_SHARED_WINDOW_HPP
#define XAML_UI_SHARED_WINDOW_HPP

#include <atomic>
#include <shared/container.hpp>
#include <xaml/ui/window.h>

#ifdef XAML_UI_WINDOWS
#include <xaml/ui/win/window.h>
#elif defined(XAML_UI_GTK3)
#include <xaml/ui/gtk3/window.h>
#endif // XAML_UI_WINDOWS

struct xaml_window_internal : xaml_container_internal
{
    std::atomic<bool> m_resizing;

    xaml_result XAML_CALL init() noexcept override;

    XAML_PROP_PTR_IMPL(menu_bar, xaml_control)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
    xaml_result XAML_CALL show() noexcept;
    xaml_result XAML_CALL close() noexcept;
    xaml_result XAML_CALL hide() noexcept;

    XAML_EVENT_IMPL(is_resizable_changed)
    XAML_PROP_EVENT_IMPL(is_resizable, bool, bool*, bool)
    XAML_EVENT_IMPL(location_changed)
    XAML_PROP_EVENT_IMPL(location, xaml_point, xaml_point*, xaml_point const&)

    xaml_result XAML_CALL get_x(double* pvalue) noexcept
    {
        *pvalue = m_location.x;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_x(double value) noexcept
    {
        if (m_location.x != value)
        {
            m_location.x = value;
            return on_location_changed(m_outer_this, m_location);
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_y(double* pvalue) noexcept
    {
        *pvalue = m_location.y;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_y(double value) noexcept
    {
        if (m_location.y != value)
        {
            m_location.y = value;
            return on_location_changed(m_outer_this, m_location);
        }
        return XAML_S_OK;
    }

    XAML_EVENT_IMPL(title_changed)
    XAML_PROP_STRING_EVENT_IMPL(title)

    XAML_EVENT_IMPL(closing)

    xaml_result XAML_CALL get_client_region(xaml_rectangle*) noexcept;
    xaml_result XAML_CALL get_dpi(double*) noexcept;

    xaml_result XAML_CALL draw_size() noexcept override;
    virtual xaml_result XAML_CALL draw_title() noexcept;
    virtual xaml_result XAML_CALL draw_child() noexcept;
    virtual xaml_result XAML_CALL draw_resizable() noexcept;
    virtual xaml_result XAML_CALL draw_menu_bar() noexcept;
#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;

    xaml_result XAML_CALL get_real_location(xaml_point*) noexcept;
    xaml_result XAML_CALL set_real_location(xaml_point const&) noexcept;

    xaml_result XAML_CALL get_real_client_region(xaml_rectangle*) noexcept;
#elif defined(XAML_UI_GTK3)
    XAML_PROP_IMPL(window_handle, GtkWidget*, GtkWidget**, GtkWidget*)
    XAML_PROP_IMPL(vbox_handle, GtkWidget*, GtkWidget**, GtkWidget*)
    XAML_PROP_IMPL(menu_bar_handle, GtkWidget*, GtkWidget**, GtkWidget*)

    static void on_destroy(GtkWidget*, xaml_window_internal*) noexcept;
    static gboolean on_delete_event(GtkWidget*, GdkEvent*, xaml_window_internal*) noexcept;
    static gboolean on_configure_event(GtkWidget*, GdkEvent*, xaml_window_internal*) noexcept;
#endif // XAML_UI_WINDOWS

    xaml_window_internal() noexcept : xaml_container_internal(), m_is_resizable(true)
    {
    }

    XAML_UI_API ~xaml_window_internal();
};

struct xaml_window_impl : xaml_container_implement<xaml_window_impl, xaml_window_internal, xaml_window>
{
    XAML_PROP_PTR_INTERNAL_IMPL(menu_bar, xaml_control)

    xaml_result XAML_CALL show() noexcept override { return m_internal.show(); }
    xaml_result XAML_CALL close() noexcept override { return m_internal.close(); }
    xaml_result XAML_CALL hide() noexcept override { return m_internal.hide(); }

    XAML_EVENT_INTERNAL_IMPL(is_resizable_changed)
    XAML_PROP_INTERNAL_IMPL(is_resizable, bool*, bool)

    XAML_EVENT_INTERNAL_IMPL(location_changed)
    XAML_PROP_INTERNAL_IMPL(location, xaml_point*, xaml_point const&)

    XAML_PROP_INTERNAL_IMPL(x, double*, double)
    XAML_PROP_INTERNAL_IMPL(y, double*, double)

    XAML_EVENT_INTERNAL_IMPL(title_changed)
    XAML_PROP_PTR_INTERNAL_IMPL(title, xaml_string)

    XAML_EVENT_INTERNAL_IMPL(closing)

    XAML_PROP_INTERNAL_IMPL_BASE(client_region, xaml_rectangle*)
    XAML_PROP_INTERNAL_IMPL_BASE(dpi, double*)

#ifdef XAML_UI_WINDOWS
    XAML_PROP_INTERNAL_IMPL(real_location, xaml_point*, xaml_point const&)

    XAML_PROP_INTERNAL_IMPL_BASE(real_client_region, xaml_rectangle*)

    struct xaml_win32_window_impl : xaml_win32_control_implement<xaml_win32_window_impl, xaml_window_impl, xaml_win32_window>
    {
        xaml_result XAML_CALL get_real_location(xaml_point* pvalue) noexcept override { return m_outer->get_real_location(pvalue); }
        xaml_result XAML_CALL set_real_location(xaml_point const& value) noexcept override { return m_outer->set_real_location(value); }
        xaml_result XAML_CALL get_real_client_region(xaml_rectangle* pvalue) noexcept override { return m_outer->get_real_client_region(pvalue); }
    } m_native_window;

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_win32_window>)
        {
            add_ref();
            *ptr = static_cast<xaml_win32_window*>(&m_native_window);
            return XAML_S_OK;
        }
        else
        {
            return xaml_container_implement::query(type, ptr);
        }
    }
#elif defined(XAML_UI_GTK3)
    XAML_PROP_INTERNAL_IMPL(window_handle, GtkWidget**, GtkWidget*)
    XAML_PROP_INTERNAL_IMPL(vbox_handle, GtkWidget**, GtkWidget*)
    XAML_PROP_INTERNAL_IMPL(menu_bar_handle, GtkWidget**, GtkWidget*)

    struct xaml_gtk3_window_impl : xaml_gtk3_control_implement<xaml_gtk3_window_impl, xaml_window_impl, xaml_gtk3_window>
    {
    } m_native_window;

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_gtk3_window>)
        {
            add_ref();
            *ptr = static_cast<xaml_gtk3_window*>(&m_native_window);
            return XAML_S_OK;
        }
        else
        {
            return xaml_container_implement::query(type, ptr);
        }
    }

#endif // XAML_UI_WINDOWS

    xaml_window_impl() noexcept : xaml_container_implement()
    {
        m_native_window.m_outer = this;
    }
};

#endif // !XAML_UI_SHARED_WINDOW_HPP
