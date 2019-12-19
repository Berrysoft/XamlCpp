#import <internal/cocoa/XamlWindow.AppDelegate.h>

@implementation XamlAppDelegate : NSObject

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
