#import <cocoa/XamlTimerDelegate.h>
#include <shared/timer.hpp>
#include <xaml/ui/timer.h>

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
    xaml_timer_impl* t = (xaml_timer_impl*)self.classPointer;
    t->on_tick();
}
@end

using namespace std;

void xaml_timer_impl::on_tick() noexcept
{
    XAML_ASSERT_SUCCEEDED(on_tick(this));
}

xaml_result xaml_timer_impl::start() noexcept
{
    if (!m_enabled.exchange(true))
    {
        XamlTimerDelegate* delegate = [[XamlTimerDelegate alloc] initWithClassPointer:this];
        NSTimer* timer = [delegate newTimer:m_interval / 1000.0];
        m_handle = timer;
    }
    return XAML_S_OK;
}

xaml_result xaml_timer_impl::stop() noexcept
{
    if (m_enabled.exchange(false))
    {
        [m_handle invalidate];
        m_handle = nil;
    }
    return XAML_S_OK;
}
