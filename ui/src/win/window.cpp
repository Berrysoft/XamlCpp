#include <shared/atomic_guard.hpp>
#include <shared/window.hpp>
#include <unordered_map>
#include <wil/resource.h>
#include <windowsx.h>
#include <xaml/internal/string.hpp>
#include <xaml/result_win32.h>
#include <xaml/ui/drawing_conv.hpp>
#include <xaml/ui/win/control.h>
#include <xaml/ui/win/dark_mode.h>
#include <xaml/ui/win/dpi.h>
#include <xaml/ui/win/font_provider.h>
#include <xaml/ui/win/window.h>
#include <xaml/ui/window.h>

#include <CommCtrl.h>
#include <Uxtheme.h>
#include <vsstyle.h>

using namespace std;

static unordered_map<HWND, xaml_window_internal*> window_map;

static wil::unique_hbrush edit_normal_back{ CreateSolidBrush(RGB(33, 33, 33)) };
constexpr COLORREF black_color{ RGB(0, 0, 0) };
constexpr COLORREF white_color{ RGB(255, 255, 255) };

LRESULT CALLBACK xaml_window_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) noexcept
{
    xaml_win32_window_message msg = { hWnd, Msg, wParam, lParam };
    auto wnd = window_map[hWnd];
    if (wnd)
    {
        LPARAM result;
        xaml_result __hr = wnd->wnd_proc(msg, &result);
        switch (msg.Msg)
        {
        case WM_NCCREATE:
            XAML_RETURN_IF_WIN32_BOOL_FALSE(XamlEnableNonClientDpiScaling(hWnd));
            break;
        case WM_CTLCOLORSTATIC:
            if (XAML_FAILED(__hr))
            {
                bool dark = XamlIsDarkModeAllowedForApp();
                HDC hDC = (HDC)wParam;
                SetBkMode(hDC, TRANSPARENT);
                if (dark)
                {
                    SetTextColor(hDC, white_color);
                    SetBkColor(hDC, black_color);
                }
                return (LRESULT)(dark ? GetStockBrush(BLACK_BRUSH) : GetStockBrush(WHITE_BRUSH));
            }
            break;
        case WM_CTLCOLOREDIT:
            if (XAML_FAILED(__hr) && XamlIsDarkModeAllowedForApp())
            {
                HDC hDC = (HDC)wParam;
                HWND hEdit = (HWND)lParam;
                SetTextColor(hDC, white_color);
                SetBkColor(hDC, black_color);
                POINT p;
                XAML_RETURN_IF_WIN32_BOOL_FALSE(GetCursorPos(&p));
                XAML_RETURN_IF_WIN32_BOOL_FALSE(ScreenToClient(hWnd, &p));
                bool isHover = hEdit == ChildWindowFromPoint(hWnd, p);
                return (intptr_t)(isHover ? GetStockBrush(BLACK_BRUSH) : edit_normal_back.get());
            }
            break;
        }
        if (XAML_SUCCEEDED(__hr)) return result;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}

xaml_result XAML_CALL xaml_window_from_native(HWND hWnd, xaml_window** ptr) noexcept
{
    if (auto wnd = window_map[hWnd])
    {
        xaml_ptr<xaml_object> obj = wnd->m_outer_this;
        return obj->query(ptr);
    }
    return XAML_E_KEYNOTFOUND;
}

xaml_window_internal::~xaml_window_internal()
{
    window_map.erase(m_handle);
    close();
}

xaml_result xaml_window_internal::draw(xaml_rectangle const&) noexcept
{
    if (!m_handle)
    {
        xaml_win32_window_create_params params = {};
        params.class_name = L"XamlWindow";
        params.style = WS_OVERLAPPEDWINDOW;
        params.x = CW_USEDEFAULT;
        params.y = CW_USEDEFAULT;
        params.width = CW_USEDEFAULT;
        params.height = CW_USEDEFAULT;
        XAML_RETURN_IF_FAILED(create(params));
        xaml_ptr<xaml_application> app;
        XAML_RETURN_IF_FAILED(xaml_application_current(&app));
        XAML_RETURN_IF_FAILED(app->window_added(static_cast<xaml_window*>(m_outer_this)));
        window_map[m_handle] = this;
        XAML_RETURN_IF_FAILED(draw_resizable());
        XAML_RETURN_IF_FAILED(draw_title());
    }
    XAML_RETURN_IF_FAILED(draw_size());
    XAML_RETURN_IF_FAILED(draw_menu_bar());
    XAML_RETURN_IF_FAILED(draw_child());
    XAML_RETURN_IF_WIN32_BOOL_FALSE(InvalidateRect(m_handle, nullptr, FALSE));
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_size() noexcept
{
    xaml_atomic_guard guard(m_resizing);
    if (!guard.test_and_set())
    {
        xaml_point real_location;
        XAML_RETURN_IF_FAILED(get_real_location(&real_location));
        xaml_size real_size;
        XAML_RETURN_IF_FAILED(get_real_size(&real_size));
        XAML_RETURN_IF_WIN32_BOOL_FALSE(SetWindowPos(m_handle, nullptr, (int)real_location.x, (int)real_location.y, (int)real_size.width, (int)real_size.height, SWP_NOZORDER));
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_title() noexcept
{
    nowide::wstackstring title;
    XAML_RETURN_IF_FAILED(to_wstring(m_title, &title));
    XAML_RETURN_IF_WIN32_BOOL_FALSE(SetWindowText(m_handle, title.c_str()));
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_child() noexcept
{
    if (m_child)
    {
        xaml_rectangle region;
        XAML_RETURN_IF_FAILED(get_client_region(&region));
        return m_child->draw(region);
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_resizable() noexcept
{
    LONG_PTR style = GetWindowLongPtr(m_handle, GWL_STYLE);
    if (m_is_resizable)
        style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
    else
        style &= (~WS_THICKFRAME) & (~WS_MAXIMIZEBOX);
    SetWindowLongPtr(m_handle, GWL_STYLE, style);
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_menu_bar() noexcept
{
    if (m_menu_bar)
    {
        XAML_RETURN_IF_FAILED(m_menu_bar->set_parent(static_cast<xaml_control*>(m_outer_this)));
        XAML_RETURN_IF_FAILED(m_menu_bar->draw({}));
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::show() noexcept
{
    XAML_RETURN_IF_FAILED(draw({}));
    ShowWindow(m_handle, SW_SHOWNORMAL);
    XAML_RETURN_IF_FAILED(set_is_visible(true));
    XAML_RETURN_IF_WIN32_BOOL_FALSE(BringWindowToTop(m_handle));
    return XAML_S_OK;
}

xaml_result xaml_window_internal::close() noexcept
{
    SendMessage(m_handle, WM_CLOSE, 0, 0);
    return XAML_S_OK;
}

xaml_result xaml_window_internal::hide() noexcept
{
    return set_is_visible(false);
}

xaml_result xaml_window_internal::get_client_region(xaml_rectangle* pregion) noexcept
{
    xaml_rectangle real_region;
    XAML_RETURN_IF_FAILED(get_real_client_region(&real_region));
    *pregion = real_region * USER_DEFAULT_SCREEN_DPI / XamlGetDpiForWindow(m_handle);
    return XAML_S_OK;
}

xaml_result xaml_window_internal::get_real_client_region(xaml_rectangle* pregion) noexcept
{
    RECT r = {};
    XAML_RETURN_IF_WIN32_BOOL_FALSE(GetClientRect(m_handle, &r));
    *pregion = xaml_from_native(r);
    return XAML_S_OK;
}

xaml_result xaml_window_internal::wnd_proc(xaml_win32_window_message const& msg, LPARAM* presult) noexcept
{
    if (m_handle && msg.hWnd == m_handle)
    {
        switch (msg.Msg)
        {
        case WM_ACTIVATE:
            XamlWindowUseDarkMode(msg.hWnd);
            break;
        case WM_ERASEBKGND:
        {
            bool dark = XamlIsDarkModeAllowedForApp();
            HDC hDC = (HDC)msg.wParam;
            HBRUSH brush = dark ? GetStockBrush(BLACK_BRUSH) : GetStockBrush(WHITE_BRUSH);
            {
                auto sresult = wil::SelectObject(hDC, brush);
                RECT r;
                XAML_RETURN_IF_WIN32_BOOL_FALSE(GetClientRect(msg.hWnd, &r));
                XAML_RETURN_IF_WIN32_BOOL_FALSE(Rectangle(hDC, r.left - 1, r.top - 1, r.right + 1, r.bottom + 1));
            }
            *presult = 1;
            return XAML_S_OK;
        }
        case WM_SIZE:
        {
            xaml_atomic_guard guard(m_resizing);
            if (!guard.test_and_set())
            {
                RECT rect = {};
                XAML_RETURN_IF_WIN32_BOOL_FALSE(GetWindowRect(m_handle, &rect));
                xaml_rectangle r = xaml_from_native(rect);
                XAML_RETURN_IF_FAILED(set_real_location({ r.x, r.y }));
                XAML_RETURN_IF_FAILED(set_real_size({ r.width, r.height }));
            }
            break;
        }
        case WM_MOVE:
        {
            xaml_atomic_guard guard(m_resizing);
            if (!guard.test_and_set())
            {
                RECT rect = {};
                XAML_RETURN_IF_WIN32_BOOL_FALSE(GetWindowRect(m_handle, &rect));
                xaml_rectangle r = xaml_from_native(rect);
                XAML_RETURN_IF_FAILED(set_real_location({ r.x, r.y }));
            }
            break;
        }
        case WM_DPICHANGED:
        {
            xaml_atomic_guard guard(m_resizing);
            if (!guard.test_and_set())
            {
                xaml_ptr<xaml_application> current_app;
                XAML_RETURN_IF_FAILED(xaml_application_current(&current_app));
                xaml_ptr<xaml_win32_font_provider> provider = current_app.query<xaml_win32_font_provider>();
                if (provider)
                {
                    HFONT font;
                    XAML_RETURN_IF_FAILED(provider->get_default_font(XamlGetDpiForWindow(m_handle), &font));
                    SendMessage(m_handle, WM_SETFONT, (WPARAM)font, TRUE);
                }
                xaml_size real_size;
                XAML_RETURN_IF_FAILED(get_real_size(&real_size));
                XAML_RETURN_IF_WIN32_BOOL_FALSE(SetWindowPos(m_handle, HWND_TOP, 0, 0, (int)real_size.width, (int)real_size.height, SWP_NOZORDER | SWP_NOMOVE));
                XAML_RETURN_IF_FAILED(draw({}));
            }
            break;
        }
        case WM_CLOSE:
        {
            xaml_ptr<xaml_object> handled;
            XAML_RETURN_IF_FAILED(xaml_box_value(false, &handled));
            XAML_RETURN_IF_FAILED(on_closing(m_outer_this, handled));
            bool value;
            XAML_RETURN_IF_FAILED(xaml_unbox_value(handled, &value));
            if (value)
            {
                *presult = 0;
                return XAML_S_OK;
            }
            break;
        }
        //case WM_LBUTTONDOWN:
        //case WM_RBUTTONDOWN:
        //case WM_MBUTTONDOWN:
        //    m_mouse_down(shared_from_this<window>(), (mouse_button)((msg.Msg - WM_LBUTTONDOWN) / 3));
        //    break;
        //case WM_LBUTTONUP:
        //case WM_RBUTTONUP:
        //case WM_MBUTTONUP:
        //    m_mouse_up(shared_from_this<window>(), (mouse_button)((msg.Msg - WM_LBUTTONUP) / 3));
        //    break;
        //case WM_MOUSEMOVE:
        //{
        //    auto real_loc = from_native(POINT{ GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam) });
        //    m_mouse_move(shared_from_this<window>(), real_loc * USER_DEFAULT_SCREEN_DPI / XamlGetDpiForWindow(get_handle()->handle));
        //    break;
        //}
        case WM_DESTROY:
        {
            xaml_ptr<xaml_application> app;
            XAML_RETURN_IF_FAILED(xaml_application_current(&app));
            XAML_RETURN_IF_FAILED(app->window_removed(static_cast<xaml_window*>(m_outer_this)));
            break;
        }
        }
        if (m_child)
        {
            xaml_ptr<xaml_win32_control> native_control = m_child.query<xaml_win32_control>();
            if (native_control)
            {
                if (XAML_SUCCEEDED(native_control->wnd_proc(msg, presult))) return XAML_S_OK;
            }
        }
        if (m_menu_bar)
        {
            xaml_ptr<xaml_win32_control> native_control = m_menu_bar.query<xaml_win32_control>();
            if (native_control)
            {
                if (XAML_SUCCEEDED(native_control->wnd_proc(msg, presult))) return XAML_S_OK;
            }
        }
    }
    return XAML_E_NOTIMPL;
}

xaml_result xaml_window_internal::get_real_location(xaml_point* plocation) noexcept
{
    *plocation = m_location * XamlGetDpiForWindow(m_handle) / USER_DEFAULT_SCREEN_DPI;
    return XAML_S_OK;
}

xaml_result xaml_window_internal::set_real_location(xaml_point const& value) noexcept
{
    return set_location(value * USER_DEFAULT_SCREEN_DPI / XamlGetDpiForWindow(m_handle));
}

xaml_result xaml_window_internal::get_dpi(double* pvalue) noexcept
{
    *pvalue = (double)XamlGetDpiForWindow(m_handle);
    return XAML_S_OK;
}
