#import <xaml/ui/cocoa/XamlDelegate.h>

@implementation XamlDelegate : NSObject
@synthesize classPointer;

- (instancetype)initWithClassPointer:(void*)ptr
{
    if ([self init])
    {
        self.classPointer = ptr;
    }
    return self;
}
@end
