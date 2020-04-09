#import <Cocoa/Cocoa.h>

@interface XamlCanvasView : NSView
{
    void* classPointer;
}
@property void* classPointer;
- (instancetype)initWithClassPointer:(void*)ptr;
@end
