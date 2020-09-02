#include <shared/progress.hpp>
#include <xaml/ui/controls/progress.h>
#include <xaml/ui/win/dpi.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_progress_internal::draw(xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent)
    {
        if (!m_handle)
        {
            xaml_win32_window_create_params params = {};
            params.class_name = PROGRESS_CLASS;
            params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD | PBS_SMOOTH;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 15;
            params.parent = parent;
            XAML_RETURN_IF_FAILED(create(params));
            XAML_RETURN_IF_FAILED(draw_visible());
            XAML_RETURN_IF_FAILED(draw_indeterminate());
        }
        XAML_RETURN_IF_FAILED(set_rect(region));
    }
    return XAML_S_OK;
}

xaml_result xaml_progress_internal::draw_progress() noexcept
{
    SendMessage(m_handle, PBM_SETRANGE32, m_minimum, m_maximum);
    SendMessage(m_handle, PBM_SETPOS, m_value, 0);
    return XAML_S_OK;
}

xaml_result xaml_progress_internal::draw_indeterminate() noexcept
{
    auto style = GetWindowLongPtr(m_handle, GWL_STYLE);
    if (m_is_indeterminate)
        style |= PBS_MARQUEE;
    else
        style &= ~PBS_MARQUEE;
    SetWindowLongPtr(m_handle, GWL_STYLE, style);
    SendMessage(m_handle, PBM_SETMARQUEE, m_is_indeterminate, 0);
    XAML_RETURN_IF_FAILED(draw_progress());
    return XAML_S_OK;
}

xaml_result xaml_progress_internal::size_to_fit() noexcept
{
    static int cyVScroll = XamlGetSystemMetricsForDpi(SM_CYVSCROLL, USER_DEFAULT_SCREEN_DPI);
    return set_size_noevent({ m_size.width, (double)cyVScroll });
}
