#include <xaml/ui/controls/progress.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void progress::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = gtk_progress_bar_new();
            set_handle(h);
            draw_visible();
            draw_progress();
        }
        __set_rect(region);
    }

    void progress::draw_progress()
    {
        gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(get_handle()->handle), (double)(m_value - m_minimum) / (double)(m_maximum - m_minimum));
    }
} // namespace xaml
