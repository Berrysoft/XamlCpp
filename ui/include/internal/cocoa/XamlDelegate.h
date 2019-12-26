#import <Cocoa/Cocoa.h>

@interface XamlDelegate : NSObject
{
    void* classPointer;
}
@property void* classPointer;
- (id)initWithClassPointer:(void*)ptr;
@end
