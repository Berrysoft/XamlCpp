#include <shared/button.hpp>
#include <xaml/ui/controls/button.h>
#include <xaml/ui/win/control.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_button_impl::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        if (!m_handle)
        {
            xaml_win32_window_create_params params = {};
            params.class_name = WC_BUTTON;
            params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;
            params.x = 0;
            params.y = 0;
            params.width = 50;
            params.height = 14;
            params.parent = m_parent;
            XAML_RETURN_IF_FAILED(create(params));
            XAML_RETURN_IF_FAILED(draw_visible());
            XAML_RETURN_IF_FAILED(draw_text());
            XAML_RETURN_IF_FAILED(draw_default());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
    }
    return XAML_S_OK;
}
