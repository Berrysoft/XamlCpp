#import <xaml/ui/cocoa/XamlDelegate.h>

@interface XamlTimerDelegate : XamlDelegate
- (NSTimer*)newTimer:(NSTimeInterval)interval;
- (void)onTick:(NSTimer*)sender;
@end
