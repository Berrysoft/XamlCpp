#include <Windows.h>
#include <wil/result_macros.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    static shared_ptr<application> _current;
    shared_ptr<application> application::current() { return _current; }

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
        cls.lpfnWndProc = wnd_callback;
        cls.cbClsExtra = 0;
        cls.hCursor = LoadCursor(nullptr, IDC_ARROW);
        cls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
        cls.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
        cls.lpszClassName = U("XamlWindow");
        cls.hInstance = GetModuleHandle(NULL);
        return RegisterClassEx(&cls);
    }

    void application::init()
    {
        _current = shared_from_this();
        THROW_IF_WIN32_BOOL_FALSE(register_window_class());
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
