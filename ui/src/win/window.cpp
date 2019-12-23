#include <internal/win/drawing.hpp>
#include <unordered_map>
#include <wil/result_macros.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    static unordered_map<HWND, weak_ptr<control>> window_map;

    LRESULT CALLBACK wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        window_message msg = { hWnd, Msg, wParam, lParam };
        auto wnd = window_map[hWnd].lock();
        auto result = wnd ? wnd->wnd_proc(msg) : nullopt;
        return result ? *result : DefWindowProc(msg.hWnd, msg.Msg, msg.wParam, msg.lParam);
    }

    window::window() : container(), m_resizable(true)
    {
        add_title_changed([this](window const&, string_view_t) { if (get_handle()) draw_title(); });
        add_location_changed([this](window const&, point) { if (get_handle() && !resizing) draw({}); });
        add_size_changed([this](control const&, size) { if (get_handle() && !resizing) draw({}); });
        add_resizable_changed([this](control const&, bool) { if(get_handle()) draw_resizable(); });
    }

    window::~window()
    {
        window_map.erase(get_handle());
    }

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
            window_map[get_handle()] = weak_from_this();
        }
        THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, get_x(), get_y(), get_width(), get_height(), SWP_NOZORDER));
        draw_resizable();
        draw_title();
        if (get_child())
        {
            draw_child();
        }
    }

    void window::draw_title()
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowText(get_handle(), m_title.c_str()));
    }

    void window::draw_child()
    {
        get_child()->draw(get_client_region());
    }

    void window::draw_resizable()
    {
        LONG_PTR style = GetWindowLongPtr(get_handle(), GWL_STYLE);
        if (m_resizable)
            style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        else
            style &= (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX);
        SetWindowLongPtr(get_handle(), GWL_STYLE, style);
    }

    void window::show()
    {
        draw({});
        ShowWindow(get_handle(), SW_SHOW);
        THROW_IF_WIN32_BOOL_FALSE(BringWindowToTop(get_handle()));
    }

    rectangle window::get_client_region() const
    {
        RECT r = {};
        THROW_IF_WIN32_BOOL_FALSE(GetClientRect(get_handle(), &r));
        return get_rect(r);
    }

    optional<LRESULT> window::wnd_proc(window_message const& msg)
    {
        switch (msg.Msg)
        {
        case WM_SIZE:
        {
            if (get_handle() && !resizing)
            {
                resizing = true;
                RECT rect = {};
                THROW_IF_WIN32_BOOL_FALSE(GetWindowRect(get_handle(), &rect));
                rectangle r = get_rect(rect);
                set_location({ r.x, r.y });
                set_size({ r.width, r.height });
                draw({});
                resizing = false;
            }
        }
        case WM_CLOSE:
        {
            bool handled = false;
            m_closing(*this, handled);
            if (handled)
            {
                return 0;
            }
            break;
        }
        case WM_DESTROY:
            application::current()->wnd_num--;
            break;
        }
        if (get_child())
            return get_child()->wnd_proc(msg);
        else
            return nullopt;
    }
} // namespace xaml
