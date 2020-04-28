#include <shared/entry.hpp>
#include <windowsx.h>
#include <xaml/ui/controls/entry.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_entry_impl::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        if (!m_handle)
        {
            xaml_win32_window_create_params params = {};
            params.class_name = WC_EDIT;
            params.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL;
            params.ex_style = WS_EX_CLIENTEDGE;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = m_parent;
            XAML_RETURN_IF_FAILED(create(params));
            XAML_RETURN_IF_FAILED(draw_visible());
            XAML_RETURN_IF_FAILED(draw_text());
            XAML_RETURN_IF_FAILED(draw_alignment());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
    }
    return XAML_S_OK;
}
