#include <Windows.h>
#include <map>
#include <wil/result_macros.h>
#include <xaml/ui/container.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    static map<HWND, control*> window_map;

    control::control() : hWnd(nullptr) {}

    void control::create(window_create_params const& params)
    {
        hWnd = CreateWindowEx(
            params.ex_style, params.class_name.c_str(), params.window_name.c_str(),
            params.style, params.x, params.y, params.width, params.height,
            params.parent ? params.parent->hWnd : nullptr,
            nullptr, GetModuleHandle(nullptr), nullptr);
        THROW_IF_NULL_ALLOC(hWnd);
        window_map[hWnd] = this;
    }

    control::~control()
    {
        SendMessage(hWnd, WM_CLOSE, 0, 0);
        window_map.erase(hWnd);
    }

    static LRESULT CALLBACK def_callback(const window_message& msg)
    {
        return DefWindowProc(msg.hWnd, msg.Msg, msg.wParam, msg.lParam);
    }

    LRESULT control::wnd_proc(const window_message& msg)
    {
        return def_callback(msg);
    }

    LRESULT CALLBACK wnd_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
    {
        window_message msg = { hWnd, Msg, wParam, lParam };
        auto wnd = window_map[hWnd];
        if (wnd)
            return wnd->wnd_proc(msg);
        else
            return def_callback(msg);
    }

    void control::set_parent(shared_ptr<container> const& value)
    {
        if (_parent != value)
        {
            if (_parent)
            {
                _parent->remove_children(shared_from_this());
            }
            if (value)
            {
                value->add_children(shared_from_this());
            }
            _parent = value;
            SetParent(hWnd, _parent->get_handle());
        }
    }

    string_t control::get_text() const
    {
        int count = GetWindowTextLength(hWnd);
        string_t result(count, L'\0');
        GetWindowText(hWnd, result.data(), count);
        return result;
    }

    void control::set_text(string_view_t value)
    {
        THROW_IF_WIN32_BOOL_FALSE(SetWindowText(hWnd, value.data()));
    }
} // namespace xaml
