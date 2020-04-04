#import <Cocoa/Cocoa.h>
#include <xaml/utility.hpp>

XAML_UI_API
@interface XamlDelegate : NSObject
{
    void* classPointer;
}
@property void* classPointer;
- (id)initWithClassPointer:(void*)ptr;
@end
