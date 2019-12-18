#include <wil/result_macros.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

namespace xaml
{
    window::window() : container()
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
        application::current()->wnd_num++;
    }

    window::~window() {}

    void window::show()
    {
        ShowWindow(hWnd, SW_SHOW);
        THROW_IF_WIN32_BOOL_FALSE(BringWindowToTop(hWnd));
    }

    LRESULT window::wnd_proc(window_message const& msg)
    {
        switch (msg.Msg)
        {
        case WM_CLOSE:
        {
            bool handled = false;
            //on_closing(*this, handled);
            if (handled)
            {
                return 0;
            }
            //ismodal = false;
            break;
        }
        case WM_DESTROY:
            application::current()->wnd_num--;
            break;
        }
        return container::wnd_proc(msg);
    }
} // namespace xaml
