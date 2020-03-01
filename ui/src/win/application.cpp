#include <Windows.h>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    static BOOL take_over_message(MSG& msg)
    {
        BOOL bRet = GetMessage(&msg, nullptr, 0, 0);
        THROW_HR_IF(E_FAIL, bRet < 0);
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
        cls.hbrBackground = GetStockBrush(WHITE_BRUSH);
        cls.lpszClassName = U("XamlWindow");
        cls.hInstance = GetModuleHandle(NULL);
        return RegisterClassEx(&cls);
    }

    static LOGFONT s_default_font;

    application::application(int argc, char_t const* const* argv) : m_cmd_lines(argv, argv + argc)
    {
        THROW_IF_WIN32_BOOL_FALSE(register_window_class());
        SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
        NONCLIENTMETRICS ncm{};
        ncm.cbSize = sizeof(ncm);
        THROW_IF_WIN32_BOOL_FALSE(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0));
        s_default_font = ncm.lfMessageFont;
        double ddpi = (double)GetDpiForSystem();
        s_default_font.lfHeight = (LONG)(s_default_font.lfHeight * 96.0 / ddpi);
        s_default_font.lfWidth = (LONG)(s_default_font.lfWidth * 96.0 / ddpi);
    }

    static HFONT get_default_font(double ddpi)
    {
        LOGFONT f = s_default_font;
        f.lfHeight = (LONG)(f.lfHeight * ddpi / 96.0);
        f.lfWidth = (LONG)(f.lfWidth * ddpi / 96.0);
        return CreateFontIndirect(&f);
    }

    HFONT application::__default_font() const
    {
        return get_default_font((double)GetDpiForSystem());
    }

    HFONT application::__default_font(HWND hWnd) const
    {
        return get_default_font((double)GetDpiForWindow(hWnd));
    }

    int application::run()
    {
        MSG msg;
        while (take_over_message(msg))
        {
            if (!m_main_wnd) PostQuitMessage(m_quit_value);
        }
        return (int)msg.wParam;
    }

    void application::quit(int value)
    {
        m_quit_value = value;
        if (m_main_wnd) DestroyWindow(m_main_wnd->get_handle()->handle);
    }
} // namespace xaml
