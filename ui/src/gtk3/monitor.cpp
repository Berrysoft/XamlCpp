#include <gtk/gtk.h>
#include <xaml/ui/drawing_conv.hpp>
#include <xaml/ui/monitor.h>

using namespace std;

xaml_result XAML_CALL xaml_monitor_get_all(xaml_vector_view<xaml_monitor>** ptr) noexcept
{
    xaml_ptr<xaml_vector<xaml_monitor>> result;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&result));
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
#else
    int n = gdk_screen_get_n_monitors(screen);
    for (int i = 0; i < n; i++)
    {
        GdkRectangle geo, work;
        gdk_screen_get_monitor_geometry(screen, i, &geo);
        gdk_screen_get_monitor_workarea(screen, i, &work);
#endif // GDK_VERSION_3_22
        XAML_RETURN_IF_FAILED(result->append({ xaml_from_native(geo), xaml_from_native(work) }));
    }
    return result->query(ptr);
}
