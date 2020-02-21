#include <Windows.h>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/meta/binding.hpp>
#include <xaml/ui/application.hpp>
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

    application::application(int argc, char_t** argv) : m_cmd_lines(argv, argv + argc)
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

    HFONT application::__default_font() const
    {
        LOGFONT f = s_default_font;
        double ddpi = (double)GetDpiForSystem();
        f.lfHeight = (LONG)(f.lfHeight * ddpi / 96.0);
        f.lfWidth = (LONG)(f.lfWidth * ddpi / 96.0);
        return CreateFontIndirect(&f);
    }

    HFONT application::__default_font(HWND hWnd) const
    {
        LOGFONT f = s_default_font;
        double ddpi = (double)GetDpiForWindow(hWnd);
        f.lfHeight = (LONG)(f.lfHeight * ddpi / 96.0);
        f.lfWidth = (LONG)(f.lfWidth * ddpi / 96.0);
        return CreateFontIndirect(&f);
    }

    int application::run()
    {
        MSG msg;
        while (take_over_message(msg))
        {
            if (!wnd_num) PostQuitMessage(0);
        }
        return (int)msg.wParam;
    }
} // namespace xaml
