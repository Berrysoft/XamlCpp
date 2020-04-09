#include <Windows.h>
#include <map>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    static BOOL take_over_message(MSG& msg)
    {
        BOOL bRet = GetMessage(&msg, nullptr, 0, 0);
        THROW_LAST_ERROR_IF(bRet < 0);
        if (bRet > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return bRet;
    }

    static BOOL register_window_class()
    {
        WNDCLASSEX cls = {};
        cls.cbSize = sizeof(WNDCLASSEX);
        cls.lpfnWndProc = __wnd_callback;
        cls.cbClsExtra = 0;
        cls.hCursor = LoadCursor(nullptr, IDC_ARROW);
        cls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        cls.hbrBackground = nullptr;
        cls.lpszClassName = U("XamlWindow");
        cls.hInstance = GetModuleHandle(NULL);
        return RegisterClassEx(&cls);
    }

    static LOGFONT s_default_font;
    static map<UINT, wil::unique_hfont> s_dpi_fonts;

    application::application(int argc, char_t const* const* argv) : m_cmd_lines(argv, argv + argc)
    {
        XamlInitializeDpiFunc();
        XamlInitializeDarkModeFunc();
        XamlSetPreferredAppMode(XAML_PREFERRED_APP_MODE_ALLOW_DARK);
        THROW_IF_WIN32_BOOL_FALSE(register_window_class());
        THROW_IF_WIN32_BOOL_FALSE(XamlSetProcessBestDpiAwareness());
        THROW_IF_WIN32_BOOL_FALSE(XamlSystemDefaultFontForDpi(&s_default_font, 96));
    }

    void* application::__default_font(unsigned int udpi) const
    {
        auto it = s_dpi_fonts.find(udpi);
        if (it != s_dpi_fonts.end())
        {
            return it->second.get();
        }
        LOGFONT f = s_default_font;
        f.lfHeight = (LONG)(f.lfHeight * (double)udpi / 96.0);
        f.lfWidth = (LONG)(f.lfWidth * (double)udpi / 96.0);
        auto p = s_dpi_fonts.emplace(udpi, CreateFontIndirect(&f));
        return p.second ? p.first->second.get() : NULL;
    }

    int application::run()
    {
        MSG msg;
        while (take_over_message(msg))
        {
            if (!m_main_wnd) PostQuitMessage(m_quit_value);
        }
        return m_quit_value;
    }

    void application::quit(int value)
    {
        m_quit_value = value;
        if (m_main_wnd) DestroyWindow(m_main_wnd->get_handle()->handle);
    }

    application_theme application::get_theme() const
    {
        return XamlIsDarkModeAllowedForApp() ? application_theme::dark : application_theme::light;
    }
} // namespace xaml
