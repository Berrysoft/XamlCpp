#include <shared/password_entry.hpp>
#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/controls/password_entry.h>

#include <CommCtrl.h>

using namespace std;

static wchar_t s_default_char{ L'*' };

xaml_result xaml_password_entry_internal::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        if (!m_handle)
        {
            xaml_win32_window_create_params params = {};
            params.class_name = WC_EDIT;
            params.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL | ES_PASSWORD;
            params.ex_style = WS_EX_CLIENTEDGE;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = m_parent;
            XAML_RETURN_IF_FAILED(create(params));
            s_default_char = Edit_GetPasswordChar(m_handle);
            XAML_RETURN_IF_FAILED(draw_password_char());
            XAML_RETURN_IF_FAILED(draw_visible());
            XAML_RETURN_IF_FAILED(draw_text());
            XAML_RETURN_IF_FAILED(draw_alignment());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
    }
    return XAML_S_OK;
}

xaml_result xaml_password_entry_internal::draw_password_char() noexcept
{
    Edit_SetPasswordChar(m_handle, m_password_char ? m_password_char : s_default_char);
    return XAML_S_OK;
}
