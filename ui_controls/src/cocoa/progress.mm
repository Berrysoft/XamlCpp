#include <xaml/ui/controls/progress.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void progress::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            NSProgressIndicator* prog = [NSProgressIndicator new];
            prog.indeterminate = NO;
            prog.usesThreadedAnimation = NO;
            auto h = make_shared<native_control>();
            h->handle = prog;
            set_handle(h);
            draw_visible();
            draw_progress();
            draw_indeterminate();
        }
        __set_rect(region);
    }

    void progress::draw_progress()
    {
        NSProgressIndicator* prog = (NSProgressIndicator*)get_handle()->handle;
        prog.minValue = (double)m_minimum;
        prog.maxValue = (double)m_maximum;
        prog.doubleValue = (double)m_value;
    }

    void progress::draw_indeterminate()
    {
        NSProgressIndicator* prog = (NSProgressIndicator*)get_handle()->handle;
        prog.indeterminate = m_is_indeterminate;
        if (m_is_indeterminate)
            [prog startAnimation:nil];
        else
            [prog stopAnimation:nil];
    }

    void progress::__int32_to_fit()
    {
        __set_size_noevent({ get_width(), 20 });
    }
}
