#import <Cocoa/Cocoa.h>
#include <xaml/ui/drawing_conv.hpp>
#include <xaml/ui/monitor.h>

using namespace std;

xaml_result XAML_CALL xaml_monitor_get_all(xaml_vector_view<xaml_monitor>** ptr) noexcept
{
    xaml_ptr<xaml_vector<xaml_monitor>> ms;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&ms));
    for (NSScreen* screen in [NSScreen screens])
    {
        NSRect const& frame = screen.frame;
        NSRect const& vframe = screen.visibleFrame;
        XAML_RETURN_IF_FAILED(ms->append({ xaml_from_native(frame), xaml_from_native(vframe) }));
    }
    return ms->query(ptr);
}
