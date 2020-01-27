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

    size control::__measure_text_size(string_view_t str) const
    {
        wil::unique_hdc_window hDC = wil::GetWindowDC(m_handle);
        SIZE s = {};
        THROW_IF_WIN32_BOOL_FALSE(GetTextExtentPoint32(hDC.get(), str.data(), (int)str.length(), &s));
        return from_native(s);
    }

    control::~control()
    {
        SendMessage(get_handle(), WM_CLOSE, 0, 0);
    }
} // namespace xaml
