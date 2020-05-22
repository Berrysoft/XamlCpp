#include <shared/control.hpp>
#include <wil/resource.h>
#include <xaml/result_win32.h>
#include <xaml/ui/win/control.h>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>
#include <xaml/ui/win/font_provider.h>

using namespace std;

xaml_result xaml_control_internal::draw_size() noexcept
{
    xaml_size real_size;
    XAML_RETURN_IF_FAILED(get_real_size(&real_size));
    XAML_RETURN_IF_WIN32_BOOL_FALSE(SetWindowPos(m_handle, nullptr, 0, 0, (int)real_size.width, (int)real_size.height, SWP_NOZORDER | SWP_NOMOVE));
    return XAML_S_OK;
}

xaml_result xaml_control_internal::draw_visible() noexcept
{
    ShowWindow(m_handle, m_is_visible ? SW_SHOWNORMAL : SW_HIDE);
    return XAML_S_OK;
}

xaml_result xaml_control_internal::create(xaml_win32_window_create_params const& params) noexcept
{
    HWND parent = nullptr;
    if (params.parent)
    {
        xaml_ptr<xaml_win32_control> native_parent;
        if (XAML_SUCCEEDED(params.parent->query(&native_parent)))
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
    return XAML_S_OK;
}

xaml_result xaml_control_internal::set_rect(xaml_rectangle const& region) noexcept
{
    xaml_rectangle real = region - m_margin;
    UINT udpi = XamlGetDpiForWindow(m_handle);
    xaml_rectangle real_real = real * udpi / USER_DEFAULT_SCREEN_DPI;
    XAML_RETURN_IF_WIN32_BOOL_FALSE(SetWindowPos(m_handle, nullptr, (int)real_real.x, (int)real_real.y, (int)real_real.width, (int)real_real.height, SWP_NOZORDER));
    return set_size_noevent({ real.width, real.height });
}

xaml_result xaml_control_internal::size_to_fit() noexcept
{
    return XAML_S_OK;
}

xaml_result xaml_control_internal::measure_string(xaml_ptr<xaml_string> const& str, xaml_size const& offset, xaml_size* pvalue) noexcept
{
    wil::unique_hdc_window hDC = wil::GetWindowDC(m_handle);
    if (hDC && str)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(str->get_data(&data));
        int32_t length;
        XAML_RETURN_IF_FAILED(str->get_length(&length));
        SIZE s = {};
        XAML_RETURN_IF_WIN32_BOOL_FALSE(GetTextExtentPoint32(hDC.get(), data, length, &s));
        *pvalue = xaml_from_native(s) + offset;
    }
    else
    {
        *pvalue = offset;
    }
    return XAML_S_OK;
}

xaml_result xaml_control_internal::get_real_size(xaml_size* pvalue) noexcept
{
    UINT udpi = XamlGetDpiForWindow(m_handle);
    *pvalue = m_size * udpi / USER_DEFAULT_SCREEN_DPI;
    return XAML_S_OK;
}

xaml_result xaml_control_internal::set_real_size(xaml_size const& value) noexcept
{
    UINT udpi = XamlGetDpiForWindow(m_handle);
    return set_size(value * USER_DEFAULT_SCREEN_DPI / udpi);
}

xaml_result xaml_control_internal::get_real_margin(xaml_margin* pvalue) noexcept
{
    UINT udpi = XamlGetDpiForWindow(m_handle);
    *pvalue = m_margin * udpi / USER_DEFAULT_SCREEN_DPI;
    return XAML_S_OK;
}

xaml_result xaml_control_internal::set_real_margin(xaml_margin const& value) noexcept
{
    UINT udpi = XamlGetDpiForWindow(m_handle);
    return set_margin(value * USER_DEFAULT_SCREEN_DPI / udpi);
}
