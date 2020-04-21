#ifndef XAML_UI_SHARED_CONTROL_HPP
#define XAML_UI_SHARED_CONTROL_HPP

#include <xaml/event.h>
#include <xaml/ui/application.h>
#include <xaml/ui/control.h>
#include <xaml/ui/native_drawing.hpp>

#ifdef XAML_UI_WINDOWS
#include <wil/resource.h>
#include <xaml/result_win32.h>
#include <xaml/ui/win/control.h>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>
#include <xaml/ui/win/font_provider.h>
#endif // XAML_UI_WINDOWS

template <typename T, typename... Base>
struct xaml_control_implement : xaml_implement<T, Base..., xaml_control, xaml_object>
{
    XAML_EVENT_IMPL(parent_changed)
    XAML_PROP_EVENT_IMPL(parent, xaml_control*, xaml_control**, xaml_control*)

    XAML_EVENT_IMPL(size_changed)
    XAML_PROP_EVENT_IMPL(size, xaml_size, xaml_size*, xaml_size const&)

    xaml_result XAML_CALL get_width(double* pvalue) noexcept override
    {
        *pvalue = m_size.width;
        return XAML_S_OK;
    }
    xaml_result XAML_CALL set_width(double value) noexcept override
    {
        if (m_size.width != value)
        {
            m_size.width = value;
            return on_size_changed(this, m_size);
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_height(double* pvalue) noexcept override
    {
        *pvalue = m_size.height;
        return XAML_S_OK;
    }
    xaml_result XAML_CALL set_height(double value) noexcept override
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

    xaml_result XAML_CALL parent_redraw() noexcept override
    {
        if (m_parent)
            return m_parent->parent_redraw();
        else
            return draw({});
    }

    xaml_result XAML_CALL set_size_noevent(xaml_size const& value) noexcept override
    {
        m_size = value;
        return XAML_S_OK;
    }

#ifdef XAML_UI_WINDOWS
    struct xaml_win32_control_impl : xaml_inner_implement<xaml_win32_control_impl, T, xaml_win32_control>
    {
        xaml_result XAML_CALL get_handle(HWND* pvalue) noexcept override { return m_outer->get_handle(pvalue); }
        xaml_result XAML_CALL set_handle(HWND value) noexcept override { return m_outer->set_handle(value); }
        xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LPARAM* res) noexcept override { return m_outer->wnd_proc(msg, res); }
        xaml_result XAML_CALL get_real_size(xaml_size* pvalue) noexcept override { return m_outer->get_real_size(pvalue); }
        xaml_result XAML_CALL set_real_size(xaml_size const& value) noexcept override { return m_outer->set_real_size(value); }
        xaml_result XAML_CALL get_real_margin(xaml_margin* pvalue) noexcept override { return m_outer->get_real_margin(pvalue); }
        xaml_result XAML_CALL set_real_margin(xaml_margin const& value) noexcept override { return m_outer->set_real_margin(value); }
    } m_native_control;

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<xaml_win32_control>)
        {
            add_ref();
            *ptr = &m_native_control;
            return XAML_S_OK;
        }
        else
        {
            return xaml_implement::query(type, ptr);
        }
    }

    xaml_result XAML_CALL size_to_fit() noexcept override
    {
        return XAML_S_OK;
    }

    XAML_PROP_IMPL(handle, HWND, HWND*, HWND)

    xaml_result XAML_CALL create(xaml_win32_window_create_params const& params) noexcept
    {
        HWND parent = nullptr;
        if (params.parent)
        {
            xaml_ptr<xaml_win32_control> native_parent;
            if (XAML_SUCCESS(params.parent->query(&native_parent)))
            {
                XAML_RETURN_IF_FAILED(native_parent->get_handle(&parent));
            }
        }
        HWND handle = CreateWindowEx(
            params.ex_style, params.class_name, params.window_name,
            params.style, params.x, params.y, params.width, params.height,
            parent, nullptr, GetModuleHandle(nullptr), nullptr);
        if (!handle) return HRESULT_FROM_WIN32(GetLastError());
        m_handle = handle;
        xaml_ptr<xaml_application> current_app;
        XAML_RETURN_IF_FAILED(xaml_application_current(&current_app));
        xaml_ptr<xaml_win32_font_provider> provider = current_app.query<xaml_win32_font_provider>();
        if (provider)
        {
            HFONT font;
            XAML_RETURN_IF_FAILED(provider->get_default_font(XamlGetDpiForWindow(m_handle), &font));
            SendMessage(m_handle, WM_SETFONT, (WPARAM)font, TRUE);
        }
        XamlControlUseDarkMode(m_handle);
    }

    xaml_result XAML_CALL set_rect(xaml_rectangle const& region) noexcept
    {
        xaml_rectangle real = region - m_margin;
        UINT udpi = XamlGetDpiForWindow(m_handle);
        xaml_rectangle real_real = real * udpi / USER_DEFAULT_SCREEN_DPI;
        XAML_RETURN_IF_WIN32_BOOL_FALSE(SetWindowPos(m_handle, nullptr, (int)real_real.x, (int)real_real.y, (int)real_real.width, (int)real_real.height, SWP_NOZORDER));
        return set_size_noevent({ real.width, real.height });
    }

    xaml_result XAML_CALL measure_string(xaml_std_string_view_t str, xaml_size offset, xaml_size* pvalue) noexcept
    {
        wil::unique_hdc_window hDC = wil::GetWindowDC(m_handle);
        if (hDC)
        {
            SIZE s = {};
            XAML_RETURN_IF_WIN32_BOOL_FALSE(GetTextExtentPoint32(hDC.get(), str.data(), (int)str.length(), &s));
            *pvalue = xaml_from_native(s) + offset;
        }
        else
        {
            *pvalue = {};
        }
        return XAML_S_OK;
    }

    virtual xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LPARAM*) noexcept
    {
        return XAML_E_NOTIMPL;
    }

    xaml_result XAML_CALL get_real_size(xaml_size* pvalue) noexcept
    {
        UINT udpi = XamlGetDpiForWindow(m_handle);
        *pvalue = m_size * udpi / USER_DEFAULT_SCREEN_DPI;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_real_size(xaml_size const& value) noexcept
    {
        UINT udpi = XamlGetDpiForWindow(m_handle);
        return set_size(value * USER_DEFAULT_SCREEN_DPI / udpi);
    }

    xaml_result XAML_CALL get_real_margin(xaml_margin* pvalue) noexcept
    {
        UINT udpi = XamlGetDpiForWindow(m_handle);
        *pvalue = m_margin * udpi / USER_DEFAULT_SCREEN_DPI;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_real_margin(xaml_margin const& value) noexcept
    {
        UINT udpi = XamlGetDpiForWindow(m_handle);
        return set_margin(value * USER_DEFAULT_SCREEN_DPI / udpi);
    }

    xaml_result XAML_CALL draw_size() noexcept
    {
        xaml_size real_size;
        XAML_RETURN_IF_FAILED(get_real_size(&real_size));
        XAML_RETURN_IF_WIN32_BOOL_FALSE(SetWindowPos(m_handle, nullptr, 0, 0, (int)real_size.width, (int)real_size.height, SWP_NOZORDER | SWP_NOMOVE));
        return XAML_S_OK;
    }

    xaml_result XAML_CALL draw_visible() noexcept
    {
        ShowWindow(m_handle, m_is_visible ? SW_SHOWNORMAL : SW_HIDE);
        return XAML_S_OK;
    }
#endif // XAML_UI_WINDOWS

    xaml_control_implement() : m_is_visible(true)
    {
        m_native_control.m_outer = this;

        XAML_THROW_IF_FAILED(xaml_event_new(&m_parent_changed));
        XAML_THROW_IF_FAILED(xaml_event_new(&m_size_changed));
        XAML_THROW_IF_FAILED(xaml_event_new(&m_margin_changed));
        XAML_THROW_IF_FAILED(xaml_event_new(&m_halignment_changed));
        XAML_THROW_IF_FAILED(xaml_event_new(&m_valignment_changed));
        XAML_THROW_IF_FAILED(xaml_event_new(&m_is_visible_changed));

        xaml_ptr<xaml_delegate> size_changed_callback;
        XAML_THROW_IF_FAILED((xaml_delegate_new<void, xaml_ptr<xaml_control>, xaml_size>(
            [this](xaml_ptr<xaml_control>, xaml_size) {
                if (m_handle)
                {
                    draw_size();
                    parent_redraw();
                }
            },
            &size_changed_callback)));
        std::size_t token;
        XAML_THROW_IF_FAILED(m_size_changed->add(size_changed_callback.get(), &token));

        xaml_ptr<xaml_delegate> is_visible_changed_callback;
        XAML_THROW_IF_FAILED((xaml_delegate_new<void, xaml_ptr<xaml_control>, bool>(
            [this](xaml_ptr<xaml_control>, bool) {
                if (m_handle) draw_visible();
            },
            &is_visible_changed_callback)));
        XAML_THROW_IF_FAILED(m_is_visible_changed->add(is_visible_changed_callback.get(), &token));
    }
};

#endif // !XAML_UI_SHARED_CONTROL_HPP
