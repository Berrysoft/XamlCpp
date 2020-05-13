#import <Cocoa/Cocoa.h>
#include <xaml/ui/monitor.h>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

xaml_result XAML_CALL xaml_monitor_get_all(xaml_vector_view** ptr) noexcept
{
    xaml_ptr<xaml_vector> ms;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&ms));
    for (NSScreen* screen in [NSScreen screens])
    {
        NSRect const& frame = screen.frame;
        NSRect const& vframe = screen.visibleFrame;
        xaml_ptr<xaml_object> obj;
        XAML_RETURN_IF_FAILED(xaml_box_value<xaml_monitor>({ xaml_from_native(frame), xaml_from_native(vframe) }));
        XAML_RETURN_IF_FAILED(ms->append(obj.get()));
    }
    return ms->query(ptr);
}
