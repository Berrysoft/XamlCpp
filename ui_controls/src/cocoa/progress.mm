#include <shared/progress.hpp>
#include <xaml/ui/controls/progress.h>

using namespace std;

xaml_result xaml_progress_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        NSProgressIndicator* prog = [NSProgressIndicator new];
        prog.indeterminate = NO;
        prog.usesThreadedAnimation = NO;
        m_handle = prog;
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_progress());
        XAML_RETURN_IF_FAILED(draw_indeterminate());
    }
    return set_rect(region);
}

xaml_result xaml_progress_internal::draw_progress() noexcept
{
    NSProgressIndicator* prog = (NSProgressIndicator*)m_handle;
    prog.minValue = (double)m_minimum;
    prog.maxValue = (double)m_maximum;
    prog.doubleValue = (double)m_value;
    return XAML_S_OK;
}

xaml_result xaml_progress_internal::draw_indeterminate() noexcept
{
    NSProgressIndicator* prog = (NSProgressIndicator*)m_handle;
    prog.indeterminate = m_is_indeterminate;
    if (m_is_indeterminate)
        [prog startAnimation:nil];
    else
        [prog stopAnimation:nil];
    return XAML_S_OK;
}

xaml_result xaml_progress_internal::size_to_fit() noexcept
{
    return set_size_noevent({ m_size.width, 20 });
}
