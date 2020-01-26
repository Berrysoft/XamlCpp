#include <Windows.h>
#include <wil/result_macros.h>
#include <windowsx.h>
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

    static HFONT s_default_font;

    void application::init_components()
    {
        THROW_IF_WIN32_BOOL_FALSE(register_window_class());
        SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE);
        NONCLIENTMETRICS ncm;
        ncm.cbSize = sizeof(ncm);
        THROW_IF_WIN32_BOOL_FALSE(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0));
        s_default_font = CreateFontIndirect(&ncm.lfMessageFont);
    }

    application::~application()
    {
    }

    HFONT application::__default_font() const
    {
        return s_default_font;
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
