#import <cocoa/XamlTimerDelegate.h>
#include <xaml/ui/native_timer.hpp>
#include <xaml/ui/timer.hpp>

@implementation XamlTimerDelegate : XamlDelegate
- (NSTimer*)newTimer:(NSTimeInterval)interval
{
    return [NSTimer scheduledTimerWithTimeInterval:interval
                                            target:self
                                          selector:@selector(onTick:)
                                          userInfo:nil
                                           repeats:YES];
}

- (void)onTick:(NSTimer*)sender
{
    xaml::timer* t = (xaml::timer*)self.classPointer;
    t->__on_tick();
}
@end

using namespace std;

namespace xaml
{
    void timer::__on_tick()
    {
        m_tick(*this);
    }

    void timer::start()
    {
        if (!m_enabled.exchange(true))
        {
            XamlTimerDelegate* delegate = [[XamlTimerDelegate alloc] initWithClassPointer:this];
            NSTimer* timer = [delegate newTimer:m_interval.count() / 1000.0];
            get_handle()->handle = timer;
        }
    }

    void timer::stop()
    {
        if (m_enabled.exchange(false))
        {
            [get_handle()->handle invalidate];
            get_handle()->handle = nil;
        }
    }
}
