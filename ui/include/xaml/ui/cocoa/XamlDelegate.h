#import <Cocoa/Cocoa.h>
#include <xaml/utility.h>

XAML_UI_API
@interface XamlDelegate : NSObject
{
    void* classPointer;
}
@property void* classPointer;
- (instancetype)initWithClassPointer:(void*)ptr;
@end
