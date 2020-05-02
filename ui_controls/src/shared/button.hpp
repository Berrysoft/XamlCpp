#ifndef XAML_UI_CONTROLS_SHARED_BUTTON_HPP
#define XAML_UI_CONTROLS_SHARED_BUTTON_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/button.h>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#include <windowsx.h>
#endif // XAML_UI_WINDOWS

template <typename T, typename... Base>
struct xaml_button_implement : xaml_control_implement<T, Base..., xaml_button>
{
    XAML_EVENT_IMPL(text_changed)
    XAML_PROP_STRING_EVENT_IMPL(text)

    XAML_EVENT_IMPL(is_default_changed)
    XAML_PROP_EVENT_IMPL(is_default, bool, bool*, bool)

    XAML_EVENT_IMPL(click)

#ifdef XAML_UI_WINDOWS
    virtual xaml_result XAML_CALL draw_text() noexcept
    {
        if (m_text)
        {
            xaml_char_t const* data;
            XAML_RETURN_IF_FAILED(m_text->get_data(&data));
            XAML_RETURN_IF_WIN32_BOOL_FALSE(Button_SetText(m_handle, data));
        }
        else
        {
            XAML_RETURN_IF_WIN32_BOOL_FALSE(Button_SetText(m_handle, nullptr));
        }
        return XAML_S_OK;
    }

    virtual xaml_result XAML_CALL draw_default() noexcept
    {
        auto style = GetWindowLongPtr(m_handle, GWL_STYLE);
        if (m_is_default)
            style |= BS_DEFPUSHBUTTON;
        else
            style &= ~BS_DEFPUSHBUTTON;
        Button_SetStyle(m_handle, style, FALSE);
        return XAML_S_OK;
    }

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const& msg, LPARAM*) noexcept override
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
                case BN_CLICKED:
                    on_click(static_cast<xaml_button*>(this));
                    break;
                }
            }
        }
        }
        return XAML_E_NOTIMPL;
    }

    xaml_result XAML_CALL size_to_fit() noexcept override
    {
        xaml_size res;
        XAML_RETURN_IF_FAILED(measure_string(m_text, { 5, 5 }, &res));
        return set_size_noevent(res);
    }
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL init() noexcept override
    {
        XAML_RETURN_IF_FAILED(xaml_control_implement::init());

        XAML_RETURN_IF_FAILED(xaml_event_new(&m_text_changed));
        XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_default_changed));
        XAML_RETURN_IF_FAILED(xaml_event_new(&m_click));

        std::int32_t token;
        XAML_RETURN_IF_FAILED((m_text_changed->add_noexcept<xaml_ptr<xaml_button>, xaml_ptr<xaml_string>>(
            [this](xaml_ptr<xaml_button>, xaml_ptr<xaml_string>) -> xaml_result {
                if (m_handle)
                {
                    XAML_RETURN_IF_FAILED(draw_text());
                    XAML_RETURN_IF_FAILED(parent_redraw());
                }
                return XAML_S_OK;
            },
            &token)));
        XAML_RETURN_IF_FAILED((m_is_default_changed->add_noexcept<xaml_ptr<xaml_button>, bool>(
            [this](xaml_ptr<xaml_button>, bool) -> xaml_result {
                if (m_handle) XAML_RETURN_IF_FAILED(draw_default());
                return XAML_S_OK;
            },
            &token)));
        return XAML_S_OK;
    }
};

struct xaml_button_impl : xaml_button_implement<xaml_button_impl>
{
    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;
};

#endif // !XAML_UI_CONTROLS_SHARED_BUTTON_HPP
