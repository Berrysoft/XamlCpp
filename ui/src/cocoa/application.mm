#import <internal/cocoa/XamlApplicationDelegate.h>
#include <xaml/ui/application.hpp>

@implementation XamlApplicationDelegate : XamlDelegate
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
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        XamlApplicationDelegate* appDelegate = [[XamlApplicationDelegate alloc] initWithClassPointer:this];
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
