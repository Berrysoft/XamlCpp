#ifndef XAML_UI_SHARED_WINDOW_HPP
#define XAML_UI_SHARED_WINDOW_HPP

#include <atomic>
#include <shared/container.hpp>
#include <xaml/ui/window.h>

#ifdef XAML_UI_WINDOWS
#include <xaml/ui/win/window.h>
#endif // XAML_UI_WINDOWS

struct xaml_window_impl : xaml_container_implement<xaml_window_impl, xaml_window>
{
protected:
    std::atomic<bool> m_resizing;

public:
    xaml_result XAML_CALL init() noexcept override;
    ~xaml_window_impl();

    XAML_PROP_PTR_IMPL(menu_bar, xaml_control)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
    xaml_result XAML_CALL show() noexcept override;
    xaml_result XAML_CALL close() noexcept override;
    xaml_result XAML_CALL hide() noexcept override;

    XAML_EVENT_IMPL(is_resizable_changed)
    XAML_PROP_EVENT_IMPL(is_resizable, bool, bool*, bool)
    XAML_EVENT_IMPL(location_changed)
    XAML_PROP_EVENT_IMPL(location, xaml_point, xaml_point*, xaml_point const&)

    xaml_result XAML_CALL get_x(double* pvalue) noexcept override
    {
        *pvalue = m_location.x;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_x(double value) noexcept override
    {
        if (m_location.x != value)
        {
            m_location.x = value;
            return on_location_changed(this, m_location);
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_y(double* pvalue) noexcept override
    {
        *pvalue = m_location.y;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_y(double value) noexcept override
    {
        if (m_location.y != value)
        {
            m_location.y = value;
            return on_location_changed(this, m_location);
        }
        return XAML_S_OK;
    }

    XAML_EVENT_IMPL(title_changed)
    XAML_PROP_PTR_EVENT_IMPL(title, xaml_string)

    XAML_EVENT_IMPL(closing)

    xaml_result XAML_CALL get_client_region(xaml_rectangle*) noexcept override;
    xaml_result XAML_CALL get_dpi(double*) noexcept override;

    xaml_result XAML_CALL draw_size() noexcept override;
    virtual xaml_result XAML_CALL draw_title() noexcept;
    virtual xaml_result XAML_CALL draw_child() noexcept;
    virtual xaml_result XAML_CALL draw_resizable() noexcept;
    virtual xaml_result XAML_CALL draw_menu_bar() noexcept;

#ifdef XAML_UI_WINDOWS
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

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LPARAM*) noexcept override;

    xaml_result XAML_CALL get_real_location(xaml_point*) noexcept;
    xaml_result XAML_CALL set_real_location(xaml_point const&) noexcept;

    xaml_result XAML_CALL get_real_client_region(xaml_rectangle*) noexcept;
#endif // XAML_UI_WINDOWS

    xaml_window_impl() noexcept : xaml_container_implement()
    {
        m_native_window.m_outer = this;
    }
};

#endif // !XAML_UI_SHARED_WINDOW_HPP
