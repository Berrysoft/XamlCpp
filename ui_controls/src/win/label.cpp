#include <shared/label.hpp>
#include <windowsx.h>
#include <xaml/result_win32.h>
#include <xaml/ui/controls/label.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_label_internal::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        if (!m_handle)
        {
            xaml_win32_window_create_params params = {};
            params.class_name = WC_STATIC;
            params.style = WS_CHILD | WS_VISIBLE | SS_LEFT;
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

xaml_result xaml_label_internal::draw_text() noexcept
{
    xaml_char_t const* data = nullptr;
    if (m_text)
    {
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
    }
    XAML_RETURN_IF_WIN32_BOOL_FALSE(Static_SetText(m_handle, data));
    return XAML_S_OK;
}

xaml_result xaml_label_internal::draw_alignment() noexcept
{
    LONG_PTR style = WS_CHILD | WS_VISIBLE;
    switch (m_text_halignment)
    {
    case xaml_halignment_center:
        style |= SS_CENTER;
        break;
    case xaml_halignment_right:
        style |= SS_RIGHT;
        break;
    default:
        style |= SS_LEFT;
        break;
    }
    SetWindowLongPtr(m_handle, GWL_STYLE, style);
    return XAML_S_OK;
}

xaml_result xaml_label_internal::size_to_fit() noexcept
{
    xaml_size res;
    XAML_RETURN_IF_FAILED(measure_string(m_text, {}, &res));
    return set_size_noevent(res);
}
