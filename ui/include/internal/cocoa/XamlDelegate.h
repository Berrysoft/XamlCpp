#import <Cocoa/Cocoa.h>
#define XAML_UI_COCOA_INTERNAL

@interface XamlDelegate : NSObject
{
    void* classPointer;
}
@property void* classPointer;
- (id)initWithClassPointer:(void*)ptr;
@end
