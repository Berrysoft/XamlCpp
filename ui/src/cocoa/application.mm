#import <cocoa/XamlApplicationDelegate.h>
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
    application::application(int argc, char_t** argv) : m_cmd_lines(argv, argv + argc)
    {
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        XamlApplicationDelegate* appDelegate = [[XamlApplicationDelegate alloc] initWithClassPointer:this];
        [[NSApplication sharedApplication] setDelegate:appDelegate];
    }

    int application::run()
    {
        [NSApp run];
        return m_quit_value;
    }

    void application::quit(int value)
    {
        m_quit_value = value;
        [NSApp terminate:nil];
    }
}
