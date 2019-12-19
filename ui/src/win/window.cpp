#include <wil/result_macros.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

namespace xaml
{
    window::window() : container()
    {
        window_create_params params = {};
        params.class_name = U("XamlWindow");
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
        ShowWindow(get_handle(), SW_SHOW);
        THROW_IF_WIN32_BOOL_FALSE(BringWindowToTop(get_handle()));
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

    string_t window::get_title() const
    {
        int count = GetWindowTextLength(get_handle());
        string_t result(count, L'\0');
        GetWindowText(get_handle(), result.data(), count);
        return result;
    }

    void window::set_title(string_view_t value)
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowText(get_handle(), value.data()));
    }
} // namespace xaml
