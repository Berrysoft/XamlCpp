#include <Cocoa/Cocoa.h>
#include <xaml/ui/control.hpp>

namespace xaml
{
    control::~control()
    {
        if (get_handle()) [get_handle() release];
    }
}
