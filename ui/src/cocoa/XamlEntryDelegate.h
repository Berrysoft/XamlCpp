#import <Cocoa/Cocoa.h>
#import <xaml/ui/cocoa/XamlDelegate.h>

@interface XamlEntryDelegate : XamlDelegate
- (void)controlTextDidChange:(NSNotification*)obj;
@end
