#ifndef XAML_UI_CONTROLS_SHARED_ENTRY_HPP
#define XAML_UI_CONTROLS_SHARED_ENTRY_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/entry.h>

#ifdef XAML_UI_WINDOWS
#include <windowsx.h>
#endif // XAML_UI_WINDOWS

template <typename T, typename... Base>
struct xaml_entry_implement : xaml_control_implement<T, Base..., xaml_entry>
{
    XAML_EVENT_IMPL(text_changed)
    XAML_PROP_STRING_EVENT_IMPL(text)

    XAML_PROP_IMPL(text_halignment, xaml_halignment, xaml_halignment*, xaml_halignment)

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL size_to_fit() noexcept override
    {
        xaml_size res;
        XAML_RETURN_IF_FAILED(measure_string(m_text, { 2, 2 }, &res));
        return set_size_noevent(res);
    }

    virtual xaml_result XAML_CALL draw_text() noexcept
    {
        if (m_text)
        {
            xaml_char_t const* data;
            XAML_RETURN_IF_FAILED(m_text->get_data(&data));
            XAML_RETURN_IF_WIN32_BOOL_FALSE(Edit_SetText(m_handle, data));
        }
        else
        {
            XAML_RETURN_IF_WIN32_BOOL_FALSE(Edit_SetText(m_handle, nullptr));
        }
        return XAML_S_OK;
    }

    virtual xaml_result XAML_CALL draw_alignment() noexcept
    {
        LONG_PTR style = WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
        switch (m_text_halignment)
        {
        case xaml_halignment_center:
            style |= ES_CENTER;
            break;
        case xaml_halignment_right:
            style |= ES_RIGHT;
            break;
        default:
            style |= ES_LEFT;
            break;
        }
        SetWindowLongPtr(m_handle, GWL_STYLE, style);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LRESULT*) noexcept override
    {
        switch (msg.Msg)
        {
        case WM_COMMAND:
        {
            HWND h = (HWND)msg.lParam;
            if (m_handle == h)
            {
                switch (HIWORD(msg.wParam))
                {
                case EN_UPDATE:
                {
                    int len = Edit_GetTextLength(m_handle);
                    xaml_std_string_t t(len, U('\0'));
                    Edit_GetText(m_handle, t.data(), len + 1);
                    DWORD start, end;
                    SendMessage(m_handle, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
                    xaml_ptr<xaml_string> text;
                    XAML_RETURN_IF_FAILED(xaml_string_new(move(t), &text));
                    XAML_RETURN_IF_FAILED(set_text(text.get()));
                    SetFocus(m_handle);
                    Edit_SetSel(m_handle, start, end);
                    break;
                }
                }
            }
        }
        }
        return XAML_E_NOTIMPL;
    }
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override
    {
        XAML_RETURN_IF_FAILED(xaml_control_implement::init());

        XAML_RETURN_IF_FAILED(xaml_event_new(&m_text_changed));

        int32_t token;
        XAML_RETURN_IF_FAILED((m_text_changed->add_noexcept<xaml_ptr<xaml_entry>, xaml_ptr<xaml_string>>(
            [this](xaml_ptr<xaml_entry>, xaml_ptr<xaml_string>) -> xaml_result {
                if (m_handle)
                {
                    XAML_RETURN_IF_FAILED(draw_text());
                    XAML_RETURN_IF_FAILED(parent_redraw());
                }
                return XAML_S_OK;
            },
            &token)));
        return XAML_S_OK;
    }
};

struct xaml_entry_impl : xaml_entry_implement<xaml_entry_impl>
{
    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_ENTRY_HPP
