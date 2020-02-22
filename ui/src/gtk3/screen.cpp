#include <gtk/gtk.h>
#include <xaml/ui/screen.hpp>

using namespace std;

namespace xaml
{
    vector<monitor> get_monitors()
    {
        vector<monitor> ms;
        GdkScreen* screen = gdk_screen_get_default();
        gdouble res = gdk_screen_get_resolution(screen);
#ifdef GDK_VERSION_3_22
        GdkDisplay* display = gdk_screen_get_display(screen);
        int n = gdk_display_get_n_monitors(display);
        for (int i = 0; i < n; i++)
        {
            GdkMonitor* m = gdk_display_get_monitor(display, i);
            GdkRectangle geo, work;
            gdk_monitor_get_geometry(m, &geo);
            gdk_monitor_get_workarea(m, &work);
            ms.push_back({ from_native(geo), from_native(work), res });
        }
#else
        int n = gdk_screen_get_n_monitors(screen);
        for (int i = 0; i < n; i++)
        {
            GdkRectangle geo, work;
            gdk_screen_get_monitor_geometry(screen, i, &geo);
            gdk_screen_get_monitor_workarea(screen, i, &work);
            ms.push_back({ from_native(geo), from_native(work), res });
        }
#endif // GDK_VERSION_3_22
        return ms;
    }
} // namespace xaml
