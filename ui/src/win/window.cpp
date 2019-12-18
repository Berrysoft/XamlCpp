#include <xaml/ui/window.hpp>
#include <wil/result_macros.h>

namespace xaml
{
    void window::create()
    {
        window_create_params params = {};
        params.class_name = U("XamlWindow");
        params.window_name = {};
        params.style = WS_OVERLAPPEDWINDOW;
        params.x = CW_USEDEFAULT;
        params.y = CW_USEDEFAULT;
        params.width = CW_USEDEFAULT;
        params.height = CW_USEDEFAULT;
        this->control::create(params);
    }

    void window::show()
    {
        if (!hWnd)
        {
            create();
        }
        ShowWindow(hWnd, SW_SHOW);
        THROW_IF_WIN32_BOOL_FALSE(BringWindowToTop(hWnd));
    }
} // namespace xaml
