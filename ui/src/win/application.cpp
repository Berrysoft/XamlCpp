#include <Windows.h>
#include <wil/result_macros.h>
#include <xaml/ui/application.hpp>

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
