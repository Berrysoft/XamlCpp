#include <wil/result_macros.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

namespace xaml
{
    static constexpr rectangle get_rect(RECT const& r) { return { (double)r.left, (double)r.top, (double)(r.right - r.left), (double)(r.bottom - r.top) }; }

    window::window() : container()
    {
    }

    window::~window() {}

    void window::draw(rectangle const& region)
    {
        if (!get_handle())
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
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, get_x(), get_y(), get_width(), get_height(), SWP_NOZORDER));
        THROW_IF_WIN32_BOOL_FALSE(SetWindowText(get_handle(), m_title.c_str()));
        if (get_child())
        {
            RECT r = {};
            THROW_IF_WIN32_BOOL_FALSE(GetClientRect(get_handle(), &r));
            get_child()->draw(get_rect(r));
        }
        ShowWindow(get_handle(), SW_SHOW);
        THROW_IF_WIN32_BOOL_FALSE(BringWindowToTop(get_handle()));
    }

    void window::show()
    {
        draw({ 0, 0, 0, 0 });
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
