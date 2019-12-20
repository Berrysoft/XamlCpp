#include <internal/cocoa/drawing.hpp>
#include <xaml/ui/screen.hpp>

using namespace std;

namespace xaml
{
    unsigned int screen_dpi()
    {
        return 72;
    }

    vector<monitor> get_monitors()
    {
        vector<monitor> ms;
        for (NSScreen* screen in [NSScreen screens])
        {
            NSRect frame = [screen frame];
            NSRect vframe = [screen visibleFrame];
            ms.push_back({ get_rect(frame), get_rect(vframe) });
        }
        return ms;
    }
}
