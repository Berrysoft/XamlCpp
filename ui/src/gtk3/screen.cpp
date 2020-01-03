#include <gtk/gtk.h>
#include <internal/gtk3/drawing.hpp>
#include <xaml/ui/screen.hpp>

using namespace std;

namespace xaml
{
    unsigned int screen_dpi()
    {
        GdkScreen* screen = gdk_screen_get_default();
        gdouble dpi = gdk_screen_get_resolution(screen);
        if (dpi < 0)
            return 96;
        else
            return (unsigned int)dpi;
    }

    vector<monitor> get_monitors()
    {
        vector<monitor> ms;
        GdkScreen* screen = gdk_screen_get_default();
#ifdef GDK_VERSION_3_22
        GdkDisplay* display = gdk_screen_get_display(screen);
        int n = gdk_display_get_n_monitors(display);
        for (int i = 0; i < n; i++)
        {
            GdkMonitor* m = gdk_display_get_monitor(display, i);
            GdkRectangle geo, work;
            gdk_monitor_get_geometry(m, &geo);
            gdk_monitor_get_workarea(m, &work);
            ms.push_back({ get_rect(geo), get_rect(work) });
        }
#else
        int n = gdk_screen_get_n_monitors(screen);
        for (int i = 0; i < n; i++)
        {
            GdkRectangle geo, work;
            gdk_screen_get_monitor_geometry(screen, i, &geo);
            gdk_screen_get_monitor_workarea(screen, i, &work);
            ms.push_back({ get_rect(geo), get_rect(work) });
        }
#endif // GDK_VERSION_3_22
        return ms;
    }
} // namespace xaml
