#ifndef XAML_UI_SHARED_CONTROL_HPP
#define XAML_UI_SHARED_CONTROL_HPP

#include <xaml/event.h>
#include <xaml/ui/application.h>
#include <xaml/ui/control.h>
#include <xaml/ui/native_drawing.hpp>

#ifdef XAML_UI_WINDOWS
#include <xaml/ui/win/control.h>
#endif // XAML_UI_WINDOWS

struct xaml_control_internal
{
    XAML_EVENT_IMPL(parent_changed)
    XAML_PROP_EVENT_IMPL(parent, xaml_control*, xaml_control**, xaml_control*)

    XAML_EVENT_IMPL(size_changed)
    XAML_PROP_EVENT_IMPL(size, xaml_size, xaml_size*, xaml_size const&)

    xaml_result XAML_CALL get_width(double* pvalue) noexcept
    {
        *pvalue = m_size.width;
        return XAML_S_OK;
    }
    xaml_result XAML_CALL set_width(double value) noexcept
    {
        if (m_size.width != value)
        {
            m_size.width = value;
            return on_size_changed(this, m_size);
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_height(double* pvalue) noexcept
    {
        *pvalue = m_size.height;
        return XAML_S_OK;
    }
    xaml_result XAML_CALL set_height(double value) noexcept
    {
        if (m_size.height != value)
        {
            m_size.height = value;
            return on_size_changed(this, m_size);
        }
        return XAML_S_OK;
    }

    XAML_EVENT_IMPL(margin_changed)
    XAML_PROP_EVENT_IMPL(margin, xaml_margin, xaml_margin*, xaml_margin const&)

    XAML_EVENT_IMPL(halignment_changed)
    XAML_PROP_EVENT_IMPL(halignment, xaml_halignment, xaml_halignment*, xaml_halignment)

    XAML_EVENT_IMPL(valignment_changed)
    XAML_PROP_EVENT_IMPL(valignment, xaml_valignment, xaml_valignment*, xaml_valignment)

    XAML_EVENT_IMPL(is_visible_changed)
    XAML_PROP_EVENT_IMPL(is_visible, bool, bool*, bool)

    XAML_UI_API xaml_result XAML_CALL parent_redraw() noexcept;

    xaml_result XAML_CALL set_size_noevent(xaml_size const& value) noexcept
    {
        m_size = value;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept;

    xaml_result XAML_CALL size_to_fit() noexcept;

    XAML_UI_API virtual xaml_result XAML_CALL draw_size() noexcept;

    XAML_UI_API virtual xaml_result XAML_CALL draw_visible() noexcept;

    xaml_result XAML_CALL set_rect(xaml_rectangle const&) noexcept;

#ifdef XAML_UI_WINDOWS
    XAML_PROP_IMPL(handle, HWND, HWND*, HWND)

    XAML_UI_API xaml_result XAML_CALL create(xaml_win32_window_create_params const&) noexcept;

    XAML_UI_API xaml_result XAML_CALL measure_string(xaml_ptr<xaml_string> const&, xaml_size const&, xaml_size*) noexcept;

    virtual xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LPARAM*) noexcept
    {
        return XAML_E_NOTIMPL;
    }

    XAML_UI_API xaml_result XAML_CALL get_real_size(xaml_size*) noexcept;
    XAML_UI_API xaml_result XAML_CALL set_real_size(xaml_size const&) noexcept;
    XAML_UI_API xaml_result XAML_CALL get_real_margin(xaml_margin*) noexcept;
    XAML_UI_API xaml_result XAML_CALL set_real_margin(xaml_margin const&) noexcept;
#endif // XAML_UI_WINDOWS

    xaml_control_internal() noexcept : m_is_visible(true)
    {
    }

    XAML_UI_API virtual xaml_result XAML_CALL init() noexcept;
};

template <typename T, typename Internal, typename... Base>
struct xaml_control_implement : xaml_implement<T, Base..., xaml_control, xaml_object>
{
    Internal m_internal;

    XAML_EVENT_INTERNAL_IMPL(parent_changed)
    XAML_PROP_INTERNAL_IMPL(parent, xaml_control**, xaml_control*)

    XAML_EVENT_INTERNAL_IMPL(size_changed)
    XAML_PROP_INTERNAL_IMPL(size, xaml_size*, xaml_size const&)

    XAML_PROP_INTERNAL_IMPL(width, double*, double)
    XAML_PROP_INTERNAL_IMPL(height, double*, double)

    XAML_EVENT_INTERNAL_IMPL(margin_changed)
    XAML_PROP_INTERNAL_IMPL(margin, xaml_margin*, xaml_margin const&)

    XAML_EVENT_INTERNAL_IMPL(halignment_changed)
    XAML_PROP_INTERNAL_IMPL(halignment, xaml_halignment*, xaml_halignment)

    XAML_EVENT_INTERNAL_IMPL(valignment_changed)
    XAML_PROP_INTERNAL_IMPL(valignment, xaml_valignment*, xaml_valignment)

    XAML_EVENT_INTERNAL_IMPL(is_visible_changed)
    XAML_PROP_INTERNAL_IMPL(is_visible, bool*, bool)

    xaml_result XAML_CALL parent_redraw() noexcept override { return m_internal.parent_redraw(); }

    xaml_result XAML_CALL set_size_noevent(xaml_size const& value) noexcept override { return m_internal.set_size_noevent(value); }

    xaml_result XAML_CALL draw(xaml_rectangle const& region) noexcept override { return m_internal.draw(region); }

    xaml_result XAML_CALL size_to_fit() noexcept override { return m_internal.size_to_fit(); }

#ifdef XAML_UI_WINDOWS
    template <typename T, typename D, typename Base>
    struct xaml_win32_control_implement : xaml_inner_implement<T, D, Base>
    {
        xaml_result XAML_CALL get_handle(HWND* pvalue) noexcept override { return m_outer->get_handle(pvalue); }
        xaml_result XAML_CALL set_handle(HWND value) noexcept override { return m_outer->set_handle(value); }
        xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LPARAM* res) noexcept override { return m_outer->wnd_proc(msg, res); }
        xaml_result XAML_CALL get_real_size(xaml_size* pvalue) noexcept override { return m_outer->get_real_size(pvalue); }
        xaml_result XAML_CALL set_real_size(xaml_size const& value) noexcept override { return m_outer->set_real_size(value); }
        xaml_result XAML_CALL get_real_margin(xaml_margin* pvalue) noexcept override { return m_outer->get_real_margin(pvalue); }
        xaml_result XAML_CALL set_real_margin(xaml_margin const& value) noexcept override { return m_outer->set_real_margin(value); }
    };

    struct xaml_win32_control_impl : xaml_win32_control_implement<xaml_win32_control_impl, T, xaml_win32_control>
    {
    } m_native_control;

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_win32_control>)
        {
            add_ref();
            *ptr = static_cast<xaml_win32_control*>(&m_native_control);
            return XAML_S_OK;
        }
        else
        {
            return xaml_implement::query(type, ptr);
        }
    }

    XAML_PROP_INTERNAL_IMPL(handle, HWND*, HWND)

    XAML_PROP_INTERNAL_IMPL(real_size, xaml_size*, xaml_size const&)
    XAML_PROP_INTERNAL_IMPL(real_margin, xaml_margin*, xaml_margin const&)

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LPARAM* presult) noexcept { return m_internal.wnd_proc(msg, presult); }
#endif // XAML_UI_WINDOWS
};

#endif // !XAML_UI_SHARED_CONTROL_HPP
