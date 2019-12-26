#import <internal/cocoa/global.h>

#include <xaml/ui/control.hpp>

namespace xaml
{
    control::~control()
    {
        if (get_handle()) [get_handle() release];
    }

    common_control::~common_control()
    {
        if (__get_delegate()) [__get_delegate() release];
    }
}
