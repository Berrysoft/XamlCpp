#import <Cocoa/Cocoa.h>
#import <xaml/ui/cocoa/XamlDelegate.h>

@interface XamlWindowDelegate : XamlDelegate <NSWindowDelegate>
- (void)onQuitAction;
@end