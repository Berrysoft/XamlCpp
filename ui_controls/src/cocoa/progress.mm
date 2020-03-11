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
            auto h = make_shared<native_control>();
            h->handle = prog;
            set_handle(h);
            draw_visible();
            draw_progress();
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
}
