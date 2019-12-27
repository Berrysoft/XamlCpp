#import <internal/cocoa/XamlDelegate.h>

@implementation XamlDelegate : NSObject
@synthesize classPointer;

- (id)initWithClassPointer:(void*)ptr
{
    if ([self init])
    {
        self.classPointer = ptr;
    }
    return self;
}
@end
