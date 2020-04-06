#include <shared/atomic_guard.hpp>
#include <unordered_map>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/application.hpp>
#include <xaml/ui/native_control.hpp>
#include <xaml/ui/native_drawing.hpp>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>
#include <xaml/ui/window.hpp>

#include <CommCtrl.h>
#include <Uxtheme.h>
#include <vsstyle.h>

using namespace std;

namespace xaml
{
    static unordered_map<HWND, weak_ptr<control>> window_map;

    static wil::unique_hbrush edit_normal_back{ CreateSolidBrush(RGB(33, 33, 33)) };
    constexpr COLORREF black_color{ RGB(0, 0, 0) };
    constexpr COLORREF white_color{ RGB(255, 255, 255) };

    LRESULT CALLBACK __wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        window_message msg = { hWnd, Msg, wParam, lParam };
        auto wnd = static_pointer_cast<window>(window_map[hWnd].lock());
        auto result = wnd ? wnd->__wnd_proc(msg) : nullopt;
        switch (msg.Msg)
        {
        case WM_NCCREATE:
            if (hWnd) THROW_IF_WIN32_BOOL_FALSE(XamlEnableNonClientDpiScaling(hWnd));
            break;
        case WM_CTLCOLORSTATIC:
            if (!result)
            {
                bool dark = XamlIsDarkModeEnabledForApp();
                HDC hDC = (HDC)wParam;
                HWND hStatic = (HWND)lParam;
                SetBkMode(hDC, TRANSPARENT);
                if (dark)
                {
                    SetTextColor(hDC, white_color);
                    SetBkColor(hDC, black_color);
                }
                result = (intptr_t)(dark ? GetStockBrush(BLACK_BRUSH) : GetStockBrush(WHITE_BRUSH));
            }
            break;
        case WM_CTLCOLOREDIT:
            if (!result && XamlIsDarkModeEnabledForApp())
            {
                HDC hDC = (HDC)wParam;
                HWND hEdit = (HWND)lParam;
                SetTextColor(hDC, white_color);
                SetBkColor(hDC, black_color);
                POINT p;
                THROW_IF_WIN32_BOOL_FALSE(GetCursorPos(&p));
                THROW_IF_WIN32_BOOL_FALSE(ScreenToClient(hWnd, &p));
                bool isHover = hEdit == ChildWindowFromPoint(hWnd, p);
                result = (intptr_t)(isHover ? GetStockBrush(BLACK_BRUSH) : edit_normal_back.get());
            }
            break;
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
            params.style = WS_OVERLAPPEDWINDOW;
            params.x = CW_USEDEFAULT;
            params.y = CW_USEDEFAULT;
            params.width = CW_USEDEFAULT;
            params.height = CW_USEDEFAULT;
            this->__create(params);
            auto shared_this = shared_from_this<window>();
            application::current()->window_added(shared_this);
            window_map[get_handle()->handle] = weak_ptr{ shared_this };
            draw_resizable();
            draw_title();
        }
        draw_size();
        draw_menu_bar();
        draw_child();
        THROW_IF_WIN32_BOOL_FALSE(InvalidateRect(get_handle()->handle, nullptr, FALSE));
    }

    void window::__parent_redraw()
    {
        __draw({});
    }

    void window::draw_size()
    {
        atomic_guard guard(m_resizing);
        if (!guard.test_and_set())
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

    void window::draw_menu_bar()
    {
        if (get_menu_bar())
        {
            get_menu_bar()->set_parent_window(shared_from_this<window>());
            get_menu_bar()->__draw({});
        }
    }

    void window::show()
    {
        __draw({});
        ShowWindow(get_handle()->handle, SW_SHOWNORMAL);
        set_is_visible(true);
        THROW_IF_WIN32_BOOL_FALSE(BringWindowToTop(get_handle()->handle));
    }

    void window::close()
    {
        SendMessage(get_handle()->handle, WM_CLOSE, 0, 0);
    }

    void window::hide()
    {
        set_is_visible(false);
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

    optional<std::intptr_t> window::__wnd_proc(window_message const& msg)
    {
        if (get_handle() && msg.hWnd == get_handle()->handle)
        {
            switch (msg.Msg)
            {
            case WM_ACTIVATE:
                if (XamlIsDarkModeEnabledForApp())
                    XamlWindowUseDarkMode(msg.hWnd);
                break;
            case WM_ERASEBKGND:
            {
                bool dark = XamlIsDarkModeEnabledForApp();
                HDC hDC = (HDC)msg.wParam;
                HBRUSH brush = dark ? GetStockBrush(BLACK_BRUSH) : GetStockBrush(WHITE_BRUSH);
                HPEN pen = dark ? GetStockPen(BLACK_PEN) : GetStockPen(WHITE_PEN);
                HBRUSH orib = SelectBrush(hDC, brush);
                HPEN orip = SelectPen(hDC, pen);
                RECT r;
                THROW_IF_WIN32_BOOL_FALSE(GetClientRect(msg.hWnd, &r));
                THROW_IF_WIN32_BOOL_FALSE(Rectangle(hDC, r.left, r.top, r.right, r.bottom));
                SelectBrush(hDC, orib);
                SelectPen(hDC, orip);
                return 1;
            }
            case WM_SIZE:
            {
                atomic_guard guard(m_resizing);
                if (!guard.test_and_set())
                {
                    RECT rect = {};
                    THROW_IF_WIN32_BOOL_FALSE(GetWindowRect(get_handle()->handle, &rect));
                    rectangle r = from_native(rect);
                    __set_real_location({ r.x, r.y });
                    __set_real_size({ r.width, r.height });
                }
                break;
            }
            case WM_MOVE:
            {
                atomic_guard guard(m_resizing);
                if (!guard.test_and_set())
                {
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
                if (!guard.test_and_set())
                {
                    SendMessage(get_handle()->handle, WM_SETFONT, (WPARAM)application::current()->__default_font(HIWORD(msg.wParam)), TRUE);
                    size real_size = __get_real_size();
                    THROW_IF_WIN32_BOOL_FALSE(SetWindowPos(get_handle()->handle, HWND_TOP, 0, 0, (int)real_size.width, (int)real_size.height, SWP_NOZORDER | SWP_NOMOVE));
                    __draw({});
                }
                break;
            }
            case WM_CLOSE:
            {
                auto handled = box_value(false);
                m_closing(shared_from_this<window>(), handled);
                if ((bool)*handled)
                {
                    return 0;
                }
                break;
            }
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_MBUTTONDOWN:
                m_mouse_down(shared_from_this<window>(), (mouse_button)((msg.Msg - WM_LBUTTONDOWN) / 3));
                break;
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            case WM_MBUTTONUP:
                m_mouse_up(shared_from_this<window>(), (mouse_button)((msg.Msg - WM_LBUTTONUP) / 3));
                break;
            case WM_MOUSEMOVE:
            {
                auto real_loc = from_native(POINT{ GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) });
                m_mouse_move(shared_from_this<window>(), real_loc * USER_DEFAULT_SCREEN_DPI / XamlGetDpiForWindow(get_handle()->handle));
                break;
            }
            case WM_DESTROY:
                application::current()->window_removed(shared_from_this<window>());
                break;
            }
            auto result = get_child() ? get_child()->__wnd_proc(msg) : nullopt;
            if (!result && get_menu_bar()) result = get_menu_bar()->__wnd_proc(msg);
            return result;
        }
        return nullopt;
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
        return (double)XamlGetDpiForWindow(get_handle()->handle);
    }
} // namespace xaml
