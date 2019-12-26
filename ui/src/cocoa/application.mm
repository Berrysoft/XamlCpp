#import <internal/cocoa/global.h>

#import <internal/cocoa/XamlApplicationDelegate.h>
#include <xaml/ui/application.hpp>

@implementation XamlApplicationDelegate : XamlDelegate

- (id)init
{
    if (self = [super init])
    {
        // allocate and initialize window and stuff here ..
    }
    return self;
}

- (void)applicationDidFinishLaunching:(NSNotification*)notification
{
    [NSApp activateIgnoringOtherApps:YES];
}

- (void)dealloc
{
    [super dealloc];
}

@end

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
        id quitMenuItem = [[[NSMenuItem alloc]
            initWithTitle:quitTitle
                   action:@selector(terminate:)
            keyEquivalent:@"q"] autorelease];
        [appMenu addItem:quitMenuItem];
        [appMenuItem setSubmenu:appMenu];

        XamlApplicationDelegate* appDelegate = [[[XamlApplicationDelegate alloc] initWithClassPointer:this] autorelease];
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
