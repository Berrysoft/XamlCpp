#import <Cocoa/Cocoa.h>
#include <vector>
#include <xaml/ui/cocoa/window.h>
#include <xaml/ui/msgbox.h>

using namespace std;

static NSAlertStyle get_style(xaml_msgbox_style style) noexcept
{
    switch (style)
    {
    case xaml_msgbox_warning:
    case xaml_msgbox_error:
        return NSAlertStyleCritical;
    case xaml_msgbox_info:
        return NSAlertStyleInformational;
    default:
        return NSAlertStyleWarning;
    }
}

xaml_result XAML_CALL xaml_msgbox_custom(xaml_window* parent, xaml_string* message, xaml_string* title, xaml_string* instruction, xaml_msgbox_style style, xaml_vector_view* buttons, xaml_msgbox_result* presult) noexcept
{
    NSWindow* parent_handle = nil;
    if (parent)
    {
        xaml_ptr<xaml_cocoa_window> native_control;
        if (XAML_SUCCEEDED(parent->query(&native_control)))
        {
            XAML_RETURN_IF_FAILED(native_control->get_window(&parent_handle));
        }
    }
    NSAlert* alert = [NSAlert new];
    if (title)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(title->get_data(&data));
        alert.window.title = [NSString stringWithUTF8String:data];
    }
    if (!instruction)
    {
        if (!message)
        {
            char const* data;
            XAML_RETURN_IF_FAILED(message->get_data(&data));
            [alert setMessageText:[NSString stringWithUTF8String:data]];
        }
    }
    else
    {
        char const* data;
        XAML_RETURN_IF_FAILED(instruction->get_data(&data));
        [alert setMessageText:[NSString stringWithUTF8String:data]];
        if (message)
        {
            char const* msg_data;
            XAML_RETURN_IF_FAILED(message->get_data(&msg_data));
            [alert setInformativeText:[NSString stringWithUTF8String:msg_data]];
        }
    }
    alert.alertStyle = get_style(style);
    vector<xaml_msgbox_result> res;
    XAML_FOREACH_START(b, buttons);
    {
        xaml_ptr<xaml_box> box;
        XAML_RETURN_IF_FAILED(b->query(&box));
        xaml_msgbox_custom_button const* button;
        XAML_RETURN_IF_FAILED(box->get_value_ptr(button));
        res.push_back(button->result);
        [alert addButtonWithTitle:[NSString stringWithUTF8String:button->text]];
    }
    XAML_FOREACH_END();
    auto ret = (ptrdiff_t)[alert runModal] - (ptrdiff_t)NSAlertFirstButtonReturn;
    if (ret < 0 || ret >= (ptrdiff_t)res.size())
        return xaml_msgbox_result_error;
    else
        return res[ret];
}
