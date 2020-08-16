#import <Cocoa/Cocoa.h>

@interface XamlCanvasView : NSView
{
    void* classPointer;
    NSTrackingArea* area;
}
@property void* classPointer;
- (instancetype)initWithClassPointer:(void*)ptr;
@end
