#include <gtk/gtk.h>
#include <xaml/ui/application.hpp>

using namespace std;

namespace xaml
{
    shared_ptr<application> application::_current;

    void application::init()
    {
        _current = shared_from_this();
        gtk_init(0, nullptr);
    }

    int application::run()
    {
        gtk_main();
        return 0;
    }

    void application::decrease_quit()
    {
        wnd_num--;
        if (!wnd_num)
            gtk_main_quit();
    }
} // namespace xaml
