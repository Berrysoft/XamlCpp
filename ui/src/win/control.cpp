#include <Windows.h>
#include <map>
#include <wil/result_macros.h>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    static map<HWND, control*> window_map;

    control::control() : hWnd(nullptr) {}

    void control::create(window_create_params const& params)
    {
        hWnd = CreateWindowEx(params.ex_style, params.class_name.c_str(), params.window_name.c_str(),
                              params.style, params.x, params.y, params.width, params.height,
                              params.parent ? params.parent->hWnd : nullptr,
                              nullptr, GetModuleHandle(nullptr), nullptr);
        THROW_IF_NULL_ALLOC(hWnd);
        window_map[hWnd] = this;
    }

    control::~control()
    {
        SendMessage(hWnd, WM_CLOSE, 0, 0);
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
        control* wnd = window_map[hWnd];
        if (wnd)
            return wnd->wnd_proc(msg);
        else
            return def_callback(msg);
    }
} // namespace xaml
