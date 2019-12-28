#import <Cocoa/Cocoa.h>
#import <xaml/ui/cocoa/XamlDelegate.h>

@interface XamlButtonDelegate : XamlDelegate
- (NSButton*)newButton;
- (void)onAction;
@end
