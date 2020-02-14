#import <Cocoa/Cocoa.h>
#import <xaml/ui/cocoa/XamlDelegate.h>

@interface XamlComboBoxDelegate : XamlDelegate <NSComboBoxDelegate>
- (void)comboBoxSelectionDidChange:(NSNotification*)obj;
@end
