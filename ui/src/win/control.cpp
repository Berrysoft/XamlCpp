#include <Windows.h>
#include <map>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/container.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    void control::__create(window_create_params const& params)
    {
        set_handle(CreateWindowEx(
            params.ex_style, params.class_name.c_str(), params.window_name.c_str(),
            params.style, params.x, params.y, params.width, params.height,
            params.parent ? params.parent->get_handle() : nullptr,
            nullptr, GetModuleHandle(nullptr), nullptr));
        THROW_IF_NULL_ALLOC(get_handle());
        SendMessage(get_handle(), WM_SETFONT, (WPARAM)application::current()->__default_font(), TRUE);
    }

    size control::__measure_text_size(string_view_t str, size offset) const
    {
        wil::unique_hdc_window hDC = wil::GetWindowDC(get_handle());
        SIZE s = {};
        THROW_IF_WIN32_BOOL_FALSE(GetTextExtentPoint32(hDC.get(), str.data(), (int)str.length(), &s));
        UINT udpi = GetDpiForWindow(get_handle());
        return from_native(s) + offset;
    }

    size control::__get_real_size() const
    {
        UINT udpi = GetDpiForWindow(get_handle());
        return get_size() * udpi / 96.0;
    }

    void control::__set_real_size(size value)
    {
        UINT udpi = GetDpiForWindow(get_handle());
        set_size(value * 96.0 / udpi);
    }

    margin control::__get_real_margin() const
    {
        UINT udpi = GetDpiForWindow(get_handle());
        return get_margin() * udpi / 96.0;
    }

    void control::__set_real_margin(margin const& value)
    {
        UINT udpi = GetDpiForWindow(get_handle());
        set_margin(value * 96.0 / udpi);
    }

    void control::__size_to_fit() {}
} // namespace xaml
