#include <Windows.h>
#include <map>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/container.hpp>
#include <xaml/ui/control.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

namespace xaml
{
    void control::__create(window_create_params const& params)
    {
        auto h = make_shared<native_control>();
        h->handle = THROW_IF_NULL_ALLOC(CreateWindowEx(
            params.ex_style, params.class_name.c_str(), params.window_name.c_str(),
            params.style, params.x, params.y, params.width, params.height,
            params.parent ? params.parent->get_handle()->handle : nullptr,
            nullptr, GetModuleHandle(nullptr), nullptr));
        set_handle(h);
        SendMessage(get_handle()->handle, WM_SETFONT, (WPARAM)application::current()->__default_font(), TRUE);
    }

    size control::__measure_text_size(string_view_t str, size offset) const
    {
        wil::unique_hdc_window hDC = wil::GetWindowDC(get_handle()->handle);
        SIZE s = {};
        THROW_IF_WIN32_BOOL_FALSE(GetTextExtentPoint32(hDC.get(), str.data(), (int)str.length(), &s));
        UINT udpi = GetDpiForWindow(get_handle()->handle);
        return from_native(s) + offset;
    }

    size control::__get_real_size() const
    {
        UINT udpi = GetDpiForWindow(get_handle()->handle);
        return get_size() * udpi / 96.0;
    }

    void control::__set_real_size(size value)
    {
        UINT udpi = GetDpiForWindow(get_handle()->handle);
        set_size(value * 96.0 / udpi);
    }

    margin control::__get_real_margin() const
    {
        UINT udpi = GetDpiForWindow(get_handle()->handle);
        return get_margin() * udpi / 96.0;
    }

    void control::__set_real_margin(margin const& value)
    {
        UINT udpi = GetDpiForWindow(get_handle()->handle);
        set_margin(value * 96.0 / udpi);
    }

    void control::__size_to_fit() {}
} // namespace xaml
