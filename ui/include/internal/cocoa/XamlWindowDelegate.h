#import <Cocoa/Cocoa.h>
#import <internal/cocoa/XamlDelegate.h>

@interface XamlWindowDelegate : XamlDelegate <NSWindowDelegate>
- (void)windowDidResize:(NSNotification*)notification;
- (BOOL)windowShouldClose:(NSWindow*)sender;
@end