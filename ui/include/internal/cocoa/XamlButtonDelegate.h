#import <Cocoa/Cocoa.h>
#import <internal/cocoa/XamlDelegate.h>

@interface XamlButtonDelegate : XamlDelegate
- (NSButton*)newButton;
- (void)onAction;
@end
