#include <xaml/ui/screen.hpp>

using namespace std;

namespace xaml
{
    vector<monitor> get_monitors()
    {
        vector<monitor> ms;
        for (NSScreen* screen in [NSScreen screens])
        {
            NSRect frame = screen.frame;
            NSRect vframe = screen.visibleFrame;
            ms.push_back({ from_native(frame), from_native(vframe) });
        }
        return ms;
    }
}
