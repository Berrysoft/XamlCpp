#include <shared/atomic_guard.hpp>
#include <unordered_map>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>
#include <xaml/ui/native_window.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    static unordered_map<HWND, weak_ptr<control>> window_map;

    static wil::unique_hbrush white_brush{ CreateSolidBrush(RGB(255, 255, 255)) };

    LRESULT CALLBACK __wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        window_message msg = { hWnd, Msg, wParam, lParam };
        auto wnd = window_map[hWnd].lock();
        auto result = wnd ? wnd->__wnd_proc(msg) : nullopt;
        if (!result)
        {
            switch (msg.Msg)
            {
            case WM_CTLCOLORSTATIC:
                result = (intptr_t)white_brush.get();
                break;
            }
        }
        return result ? *result : DefWindowProc(msg.hWnd, msg.Msg, msg.wParam, msg.lParam);
    }

    shared_ptr<window> __get_window(HWND hWnd)
    {
        if (auto wnd = window_map[hWnd].lock())
        {
            return reinterpret_pointer_cast<window>(wnd);
        }
        return nullptr;
    }

    window::~window()
    {
        close();
        window_map.erase(get_handle()->handle);
    }

    void window::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            window_create_params params = {};
            params.class_name = U("XamlWindow");
            params.style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
            params.x = CW_USEDEFAULT;
            params.y = CW_USEDEFAULT;
            params.width = CW_USEDEFAULT;
            params.height = CW_USEDEFAULT;
            this->__create(params);
            auto shared_this = static_pointer_cast<window>(shared_from_this());
            application::current()->window_added(shared_this);
            window_map[get_handle()->handle] = weak_ptr{ shared_this };
            set_window(make_shared<native_window>());
            draw_resizable();
            draw_title();
        }
        draw_size();
        draw_child();
        auto wnd_dc = wil::GetDC(get_handle()->handle);
        get_window()->store_dc.reset(CreateCompatibleDC(wnd_dc.get()));
        rectangle cr = __get_real_client_region();
        wil::unique_hbitmap bitmap{ CreateCompatibleBitmap(wnd_dc.get(), (int)cr.width, (int)cr.height) };
        wil::unique_hbitmap ori_bitmap{ SelectBitmap(get_window()->store_dc.get(), bitmap.release()) };
        THROW_IF_WIN32_BOOL_FALSE(InvalidateRect(get_handle()->handle, nullptr, FALSE));
    }

    void window::__parent_redraw()
    {
        __draw({});
    }

    void window::draw_size()
    {
        atomic_guard guard(m_resizing);
        if (!guard.exchange(true))
        {
            point real_location = __get_real_location();
            size real_size = __get_real_size();
            THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle()->handle, HWND_TOP, (int)real_location.x, (int)real_location.y, (int)real_size.width, (int)real_size.height, SWP_NOZORDER));
        }
    }

    void window::draw_title()
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowText(get_handle()->handle, m_title.c_str()));
    }

    void window::draw_child()
    {
        if (get_child())
            get_child()->__draw(get_client_region());
    }

    void window::draw_resizable()
    {
        LONG_PTR style = GetWindowLongPtr(get_handle()->handle, GWL_STYLE);
        if (m_resizable)
            style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
        else
            style &= (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX);
        SetWindowLongPtr(get_handle()->handle, GWL_STYLE, style);
    }

    void window::show()
    {
        __draw({});
        ShowWindow(get_handle()->handle, SW_SHOWNORMAL);
        THROW_IF_WIN32_BOOL_FALSE(BringWindowToTop(get_handle()->handle));
    }

    void window::close()
    {
        SendMessage(get_handle()->handle, WM_CLOSE, 0, 0);
    }

    void window::hide()
    {
        ShowWindow(get_handle()->handle, SW_HIDE);
    }

    rectangle window::get_client_region() const
    {
        return __get_real_client_region() * 96.0 / get_dpi();
    }

    rectangle window::__get_real_client_region() const
    {
        RECT r = {};
        THROW_IF_WIN32_BOOL_FALSE(GetClientRect(get_handle()->handle, &r));
        return from_native(r);
    }

    void window::__copy_hdc(rectangle const& region, HDC hDC)
    {
        THROW_IF_WIN32_BOOL_FALSE(BitBlt(get_window()->store_dc.get(), (int)region.x, (int)region.y, (int)region.width, (int)region.height, hDC, 0, 0, SRCCOPY));
    }

    optional<std::intptr_t> window::__wnd_proc(window_message const& msg)
    {
        PAINTSTRUCT ps;
        wil::unique_hdc_paint hDC;
        switch (msg.Msg)
        {
        case WM_SIZE:
        {
            atomic_guard guard(m_resizing);
            if (get_handle() && !guard.exchange(true))
            {
                double udpi = get_dpi();
                RECT rect = {};
                THROW_IF_WIN32_BOOL_FALSE(GetWindowRect(get_handle()->handle, &rect));
                rectangle r = from_native(rect);
                __set_real_location({ r.x, r.y });
                __set_real_size({ r.width, r.height });
                __draw({});
            }
            break;
        }
        case WM_MOVE:
        {
            atomic_guard guard(m_resizing);
            if (get_handle() && !guard.exchange(true))
            {
                double udpi = get_dpi();
                RECT rect = {};
                THROW_IF_WIN32_BOOL_FALSE(GetWindowRect(get_handle()->handle, &rect));
                rectangle r = from_native(rect);
                __set_real_location({ r.x, r.y });
            }
            break;
        }
        case WM_DPICHANGED:
        {
            atomic_guard guard(m_resizing);
            if (get_handle() && !guard.exchange(true))
            {
                size real_size = __get_real_size();
                THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle()->handle, HWND_TOP, 0, 0, (int)real_size.width, (int)real_size.height, SWP_NOZORDER | SWP_NOMOVE));
                SendMessage(get_handle()->handle, WM_SETFONT, (WPARAM)application::current()->__default_font(), TRUE);
                RECT rect = {};
                THROW_IF_WIN32_BOOL_FALSE(GetWindowRect(get_handle()->handle, &rect));
                rectangle r = from_native(rect);
                __set_real_location({ r.x, r.y });
                __draw({});
            }
            break;
        }
        case WM_PAINT:
        {
            hDC = wil::BeginPaint(get_handle()->handle, &ps);
            rectangle region = __get_real_client_region();
            THROW_IF_WIN32_BOOL_FALSE(Rectangle(get_window()->store_dc.get(), (int)region.x - 1, (int)region.y - 1, (int)region.width + 1, (int)region.height + 1));
            auto result = get_child() ? get_child()->__wnd_proc(msg) : nullopt;
            THROW_IF_WIN32_BOOL_FALSE(BitBlt(hDC.get(), (int)region.x, (int)region.y, (int)region.width, (int)region.height, get_window()->store_dc.get(), 0, 0, SRCCOPY));
            return result;
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
            application::current()->window_removed(static_pointer_cast<window>(shared_from_this()));
            break;
        }
        return get_child() ? get_child()->__wnd_proc(msg) : nullopt;
    }

    point window::__get_real_location() const
    {
        return get_location() * get_dpi() / 96.0;
    }

    void window::__set_real_location(point value)
    {
        set_location(value * 96.0 / get_dpi());
    }

    double window::get_dpi() const
    {
        return (double)GetDpiForWindow(get_handle()->handle);
    }
} // namespace xaml
