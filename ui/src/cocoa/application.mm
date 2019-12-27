#import <internal/cocoa/global.h>

#include <xaml/ui/application.hpp>

@implementation XamlApplicationDelegate : XamlDelegate

- (id)init
{
    if (self = [super init])
    {
    }
    return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
    [NSApp activateIgnoringOtherApps:YES];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication*)theApplication
{
    return YES;
}

@end

using namespace std;

namespace xaml
{
    void application::init_components()
    {
        [NSApplication sharedApplication];
#ifndef XAML_UI_USE_GNUSTEP
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
#endif // !XAML_UI_USE_GNUSTEP
        XamlApplicationDelegate* appDelegate = [[XamlApplicationDelegate alloc] initWithClassPointer:this];
        __set_delegate(appDelegate);
        [[NSApplication sharedApplication] setDelegate:appDelegate];
    }

    int application::run()
    {
        [NSApp run];
        return 0;
    }

    application::~application()
    {
        [NSApp terminate:nil];
    }
}
