#pragma comment(linker, "\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <Windows.h>
#include <gdiplus.h>
#include <shellapi.h>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

using namespace std;
using namespace Gdiplus;

namespace xaml
{
#ifdef UNICODE
    application::application(LPWSTR lpCmdLine)
    {
        int argc;
        LPWSTR* argv = CommandLineToArgvW(lpCmdLine, &argc);
        if (argv)
        {
            wil::unique_array_ptr<LPWSTR, wil::hlocal_deleter> args(argv, argc);
            for (int i = 0; i < argc; i++)
            {
                m_cmd_lines.push_back(args[i]);
            }
        }
    }
#endif // UNICODE

    extern shared_ptr<application> s_current;

    shared_ptr<application> application::init(LPWSTR lpCmdLine)
    {
        s_current = shared_ptr<application>(new application(lpCmdLine));
        s_current->init_components();
        return s_current;
    }

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
        GdiplusStartupInput gdiplusStartupInput;
        GdiplusStartup(&m_gdiplus_oken, &gdiplusStartupInput, NULL);
    }

    application::~application()
    {
        GdiplusShutdown(m_gdiplus_oken);
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
