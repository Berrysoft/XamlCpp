#import <cocoa/XamlButtonDelegate.h>
#include <shared/button.hpp>
#include <xaml/ui/controls/button.h>
#include <xaml/ui/native_drawing.hpp>

@implementation XamlButtonDelegate : XamlDelegate
- (void)onAction
{
    xaml_button_internal* ptr = (xaml_button_internal*)self.classPointer;
    ptr->on_action();
}
@end

using namespace std;

xaml_result xaml_button_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        XamlButtonDelegate* delegate = [[XamlButtonDelegate alloc] initWithClassPointer:this];
        NSButtonCell* button = [NSButtonCell new];
        button.target = delegate;
        button.action = @selector(onAction);
        [button setBezelStyle:NSBezelStyleRounded];
        NSMatrix* matrix = [[NSMatrix alloc] initWithFrame:NSMakeRect(0, 0, 1, 1)
                                                      mode:NSTrackModeMatrix
                                                 prototype:(NSCell*)button
                                              numberOfRows:1
                                           numberOfColumns:1];
        matrix.autorecalculatesCellSize = YES;
        m_handle = matrix;
        m_delegate = delegate;
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_default());
    }
    XAML_RETURN_IF_FAILED(set_rect(region));
    NSMatrix* matrix = (NSMatrix*)m_handle;
    matrix.cellSize = matrix.frame.size;
    return XAML_S_OK;
}

xaml_result xaml_button_internal::draw_text() noexcept
{
    NSMatrix* matrix = (NSMatrix*)m_handle;
    NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
    NSString* ns_title;
    if (m_text)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
        ns_title = [NSString stringWithUTF8String:data];
	}
    else
    {
        ns_title = @"";
	}
    button.title = ns_title;
    return XAML_S_OK;
}

xaml_result xaml_button_internal::draw_size() noexcept
{
    NSMatrix* matrix = (NSMatrix*)m_handle;
    NSRect frame = matrix.frame;
    frame.size = xaml_to_native<NSSize>(m_size);
    matrix.frame = frame;
    matrix.cellSize = frame.size;
    return XAML_S_OK;
}

xaml_result xaml_button_internal::draw_default() noexcept
{
    NSMatrix* matrix = (NSMatrix*)m_handle;
    NSButtonCell* button = (NSButtonCell*)[matrix.cells objectAtIndex:0];
    if (m_is_default)
    {
        button.keyEquivalent = @"\r";
        button.highlighted = YES;
    }
    else
    {
        button.keyEquivalent = @"";
        button.highlighted = NO;
    }
    return XAML_S_OK;
}

void xaml_button_internal::on_action() noexcept
{
    XAML_ASSERT_SUCCEEDED(on_click(m_outer_this));
}
