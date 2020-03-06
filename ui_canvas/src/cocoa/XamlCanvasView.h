#import <Cocoa/Cocoa.h>

@interface XamlCanvasView : NSView
{
    void* classPointer;
}
@property void* classPointer;
- (id)initWithClassPointer:(void*)ptr;
@end
