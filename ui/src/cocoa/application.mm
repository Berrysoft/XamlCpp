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
    application::application(int argc, char_t const* const* argv) : m_cmd_lines(argv, argv + argc)
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

    application_theme application::get_theme() const
    {
        NSString *osxMode = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];
        if ([osxMode isEqualToString:@"Dark"])
        {
            return application_theme::dark;
		}
        return application_theme::light;
    }
}
