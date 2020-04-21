#ifndef XAML_UI_SHARED_CONTROL_HPP
#define XAML_UI_SHARED_CONTROL_HPP

#include <xaml/ui/application.h>
#include <xaml/ui/control.h>

#ifdef XAML_UI_WINDOWS
#include <xaml/ui/win/control.h>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>
#include <xaml/ui/win/font_provider.h>
#endif // XAML_UI_WINDOWS

template <typename T, typename... Base>
struct xaml_control_implement
    : xaml_implement<T, Base..., xaml_control, xaml_object
#ifdef XAML_UI_WINDOWS
                     ,
                     xaml_win32_control
#endif // XAML_UI_WINDOWS
                     >
#ifdef XAML_UI_WINDOWS
    ,
      xaml_win32_control
#endif // XAML_UI_WINDOWS
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

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LPARAM*) noexcept override
    {
        return XAML_E_NOTIMPL;
    }

    xaml_result XAML_CALL get_real_size(xaml_size* pvalue) noexcept override
    {
        UINT udpi = XamlGetDpiForWindow(m_handle);
        *pvalue = m_size * udpi / USER_DEFAULT_SCREEN_DPI;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_real_size(xaml_size const& value) noexcept override
    {
        UINT udpi = XamlGetDpiForWindow(m_handle);
        return set_size(value * USER_DEFAULT_SCREEN_DPI / udpi);
    }

    xaml_result XAML_CALL get_real_margin(xaml_margin* pvalue) noexcept override
    {
        UINT udpi = XamlGetDpiForWindow(m_handle);
        *pvalue = m_margin * udpi / USER_DEFAULT_SCREEN_DPI;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_real_margin(xaml_margin const& value) noexcept override
    {
        UINT udpi = XamlGetDpiForWindow(m_handle);
        return set_margin(value * USER_DEFAULT_SCREEN_DPI / udpi);
    }
#endif // XAML_UI_WINDOWS
};

#endif // !XAML_UI_SHARED_CONTROL_HPP
