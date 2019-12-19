#import <Cocoa/Cocoa.h>
#import <internal/cocoa/XamlWindow.AppDelegate.h>
#include <xaml/ui/application.hpp>

namespace xaml
{
    static shared_ptr<application> _current;
    shared_ptr<application> application::current() { return _current; }

    void application::init()
    {
        _current = shared_from_this();

        [NSAutoreleasePool new];
        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
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
    }
}
