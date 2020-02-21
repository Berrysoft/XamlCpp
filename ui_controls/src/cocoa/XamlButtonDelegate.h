#import <Cocoa/Cocoa.h>
#import <xaml/ui/cocoa/XamlDelegate.h>

@interface XamlButtonDelegate : XamlDelegate
- (NSMatrix*)newButton;
- (void)onAction;
@end
