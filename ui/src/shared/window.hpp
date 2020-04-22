#ifndef XAML_UI_SHARED_WINDOW_HPP
#define XAML_UI_SHARED_WINDOW_HPP

#include <atomic>
#include <shared/container.hpp>
#include <xaml/ui/window.h>

struct xaml_window_impl : xaml_container_implement<xaml_window_impl, xaml_window>
{
protected:
    std::atomic<bool> m_resizing;

public:
    xaml_window_impl();
    ~xaml_window_impl();

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

    xaml_result draw_size() noexcept override;
    virtual xaml_result draw_title() noexcept;
    virtual xaml_result draw_child() noexcept;
    virtual xaml_result draw_resizable() noexcept;
    virtual xaml_result draw_menu_bar() noexcept;

#ifdef XAML_UI_WINDOWS
    xaml_result wnd_proc(xaml_win32_window_message const&, LPARAM*) noexcept override;

    xaml_result get_real_location(xaml_point*) noexcept;
    xaml_result set_real_location(xaml_point const&) noexcept;

    xaml_result get_real_client_region(xaml_rectangle*) noexcept;
#endif // XAML_UI_WINDOWS
};

#endif // !XAML_UI_SHARED_WINDOW_HPP
