#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/controls/entry.hpp>
#include <xaml/ui/native_control.hpp>

#include <CommCtrl.h>

using namespace std;

namespace xaml
{
    optional<std::intptr_t> entry::__wnd_proc(window_message const& msg)
    {
        switch (msg.Msg)
        {
        case WM_COMMAND:
        {
            HWND h = (HWND)msg.lParam;
            if (get_handle() && get_handle()->handle == h)
            {
                switch (HIWORD(msg.wParam))
                {
                case EN_UPDATE:
                {
                    int len = Edit_GetTextLength(get_handle()->handle);
                    string_t t(len, U('\0'));
                    Edit_GetText(get_handle()->handle, t.data(), len + 1);
                    DWORD start, end;
                    SendMessage(get_handle()->handle, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
                    set_text(t);
                    SetFocus(get_handle()->handle);
                    Edit_SetSel(get_handle()->handle, start, end);
                    break;
                }
                }
            }
        }
        }
        return nullopt;
    }

    void entry::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            if (!get_handle())
            {
                window_create_params params = {};
                params.class_name = WC_EDIT;
                params.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL;
                params.ex_style = WS_EX_CLIENTEDGE;
                params.x = 0;
                params.y = 0;
                params.width = 100;
                params.height = 50;
                params.parent = sparent.get();
                this->__create(params);
                draw_text();
                draw_alignment();
                SetParent(get_handle()->handle, sparent->get_handle()->handle);
            }
            __set_rect(region);
        }
    }

    void entry::draw_text()
    {
        THROW_IF_WIN32_BOOL_FALSE(Edit_SetText(get_handle()->handle, m_text.c_str()));
    }

    void entry::draw_alignment()
    {
        LONG_PTR style = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
        switch (m_text_halignment)
        {
        case halignment_t::center:
            style |= ES_CENTER;
            break;
        case halignment_t::right:
            style |= ES_RIGHT;
            break;
        default:
            style |= ES_LEFT;
            break;
        }
        SetWindowLongPtr(get_handle()->handle, GWL_STYLE, style);
    }

    void entry::__size_to_fit()
    {
        __set_size_noevent(__measure_text_size(m_text, { 2, 2 }));
    }
} // namespace xaml
