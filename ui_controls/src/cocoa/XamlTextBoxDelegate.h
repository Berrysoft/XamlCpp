#import <Cocoa/Cocoa.h>
#import <xaml/ui/cocoa/XamlDelegate.h>

@interface XamlTextBoxDelegate : XamlDelegate<NSTextViewDelegate>
- (void)textDidChange:(NSNotification*)obj;
@end
