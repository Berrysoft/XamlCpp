#import <Cocoa/Cocoa.h>
#import <internal/cocoa/XamlWindow.AppDelegate.h>
#include <xaml/ui/application.hpp>

using namespace std;

namespace xaml
{
    void application::init_components()
    {
        [NSAutoreleasePool new];
        [NSApplication sharedApplication];
        // [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        id menubar = [[NSMenu new] autorelease];
        id appMenuItem = [[NSMenuItem new] autorelease];
        [menubar addItem:appMenuItem];
        [NSApp setMainMenu:menubar];
        id appMenu = [[NSMenu new] autorelease];
        id appName = [[NSProcessInfo processInfo] processName];
        id quitTitle = [@"Quit " stringByAppendingString:appName];
        id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:quitTitle
                                                      action:@selector(terminate:)
                                               keyEquivalent:@"q"] autorelease];
        [appMenu addItem:quitMenuItem];
        [appMenuItem setSubmenu:appMenu];

        XamlAppDelegate* appDelegate = [[[XamlAppDelegate alloc] init] autorelease];
        NSApplication* application = [NSApplication sharedApplication];
        [application setDelegate:appDelegate];
    }

    int application::run()
    {
        [NSApp run];
        return 0;
    }
}
