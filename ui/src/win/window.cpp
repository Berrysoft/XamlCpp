#include <shared/atomic_guard.hpp>
#include <unordered_map>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    static unordered_map<HWND, weak_ptr<control>> window_map;

    LRESULT CALLBACK __wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        window_message msg = { hWnd, Msg, wParam, lParam };
        auto wnd = window_map[hWnd].lock();
        auto result = wnd ? wnd->__wnd_proc(msg) : nullopt;
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
        window_map.erase(get_handle());
    }

    void window::__draw(rectangle const& region)
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
            this->__create(params);
            application::current()->wnd_num++;
            window_map[get_handle()] = weak_from_this();
        }
        {
            atomic_guard guard(m_resizing);
            if (!guard.exchange(true))
            {
                UINT udpi = GetDpiForWindow(get_handle());
                THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, (int)(get_x() * udpi / 96.0), (int)(get_y() * udpi / 96.0), (int)(get_width() * udpi / 96.0), (int)(get_height() * udpi / 96.0), SWP_NOZORDER));
            }
        }
        draw_resizable();
        draw_title();
        auto wnd_dc = wil::GetDC(get_handle());
        m_store_dc.reset(CreateCompatibleDC(wnd_dc.get()));
        rectangle cr = get_client_region();
        wil::unique_hbitmap bitmap{ CreateCompatibleBitmap(wnd_dc.get(), (int)cr.width, (int)cr.height) };
        wil::unique_hbitmap ori_bitmap{ SelectBitmap(m_store_dc.get(), bitmap.release()) };
        if (get_child())
        {
            draw_child();
        }
        THROW_IF_WIN32_BOOL_FALSE(InvalidateRect(get_handle(), nullptr, FALSE));
    }

    void window::__parent_redraw()
    {
        __draw({});
    }

    void window::draw_title()
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowText(get_handle(), m_title.c_str()));
    }

    void window::draw_child()
    {
        get_child()->__draw(get_client_region());
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
        __draw({});
        ShowWindow(get_handle(), SW_SHOW);
        THROW_IF_WIN32_BOOL_FALSE(BringWindowToTop(get_handle()));
    }

    void window::close()
    {
        SendMessage(get_handle(), WM_CLOSE, 0, 0);
    }

    rectangle window::get_client_region() const
    {
        RECT r = {};
        THROW_IF_WIN32_BOOL_FALSE(GetClientRect(get_handle(), &r));
        return from_native(r);
    }

    void window::__copy_hdc(rectangle const& region, HDC hDC)
    {
        THROW_IF_WIN32_BOOL_FALSE(BitBlt(m_store_dc.get(), (int)region.x, (int)region.y, (int)region.width, (int)region.height, hDC, 0, 0, SRCCOPY));
    }

    optional<LRESULT> window::__wnd_proc(window_message const& msg)
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
                UINT udpi = GetDpiForWindow(get_handle());
                RECT rect = {};
                THROW_IF_WIN32_BOOL_FALSE(GetWindowRect(get_handle(), &rect));
                rectangle r = from_native(rect);
                set_location(point{ r.x, r.y } * 96.0 / udpi);
                set_size(size{ r.width, r.height } * 96.0 / udpi);
                __draw({});
            }
            break;
        }
        case WM_MOVE:
        {
            atomic_guard guard(m_resizing);
            if (get_handle() && !guard.exchange(true))
            {
                UINT udpi = GetDpiForWindow(get_handle());
                RECT rect = {};
                THROW_IF_WIN32_BOOL_FALSE(GetWindowRect(get_handle(), &rect));
                rectangle r = from_native(rect);
                set_location(point{ r.x, r.y } * 96.0 / udpi);
            }
            break;
        }
        case WM_DPICHANGED:
        {
            atomic_guard guard(m_resizing);
            if (get_handle() && !guard.exchange(true))
            {
                UINT udpi = GetDpiForWindow(get_handle());
                THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle(), HWND_TOP, (int)(get_x() * udpi / 96.0), (int)(get_y() * udpi / 96.0), (int)(get_width() * udpi / 96.0), (int)(get_height() * udpi / 96.0), SWP_NOZORDER));
                __draw({});
            }
            break;
        }
        case WM_PAINT:
        {
            hDC = wil::BeginPaint(get_handle(), &ps);
            rectangle region = get_client_region();
            THROW_IF_WIN32_BOOL_FALSE(Rectangle(m_store_dc.get(), (int)region.x - 1, (int)region.y - 1, (int)region.width + 1, (int)region.height + 1));
            auto result = get_child() ? get_child()->__wnd_proc(msg) : nullopt;
            THROW_IF_WIN32_BOOL_FALSE(BitBlt(hDC.get(), (int)region.x, (int)region.y, (int)region.width, (int)region.height, m_store_dc.get(), 0, 0, SRCCOPY));
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
            application::current()->wnd_num--;
            break;
        }
        return get_child() ? get_child()->__wnd_proc(msg) : nullopt;
    }
} // namespace xaml
