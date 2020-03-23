#include <win/pinvoke.h>
#include <xaml/ui/controls/progress.hpp>
#include <xaml/ui/native_control.hpp>

#include <CommCtrl.h>

using namespace std;

namespace xaml
{
    void progress::__draw(rectangle const& region)
    {
        if (auto sparent = get_parent().lock())
        {
            if (!get_handle())
            {
                window_create_params params = {};
                params.class_name = PROGRESS_CLASS;
                params.style = WS_TABSTOP | WS_VISIBLE | WS_CHILD;
                params.x = 0;
                params.y = 0;
                params.width = 100;
                params.height = 15;
                params.parent = sparent.get();
                this->__create(params);
                draw_visible();
                draw_progress();
                SetParent(get_handle()->handle, sparent->get_handle()->handle);
            }
            __set_rect(region);
        }
    }

    void progress::draw_progress()
    {
        SendMessage(get_handle()->handle, PBM_SETRANGE32, m_minimum, m_maximum);
        SendMessage(get_handle()->handle, PBM_SETPOS, m_value, 0);
    }

    void progress::__size_to_fit()
    {
        static int cyVScroll = XamlGetSystemMetricsForDpi(SM_CYVSCROLL, 96);
        __set_size_noevent({ get_width(), (double)cyVScroll });
    }
} // namespace xaml
