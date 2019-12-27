#include <xaml/ui/msgbox.hpp>

using namespace std;

#ifdef XAML_UI_USE_GNUSTEP
constexpr NSAlertStyle NSAlertStyleCritical = NSCriticalAlertStyle;
constexpr NSAlertStyle NSAlertStyleInformational = NSInformationalAlertStyle;
constexpr NSAlertStyle NSAlertStyleWarning = NSWarningAlertStyle;

typedef NSInteger NSModalResponse;
#endif // XAML_UI_USE_GNUSTEP

namespace xaml
{
    NSAlertStyle get_style(msgbox_style style)
    {
        switch (style)
        {
        case msgbox_style::warning:
        case msgbox_style::error:
            return NSAlertStyleWarning;
        case msgbox_style::info:
            return NSAlertStyleInformational;
        default:
            return NSAlertStyleCritical;
        }
    }

    msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        NSAlert* alert = [NSAlert new];
        NSString* title_str = [NSString stringWithUTF8String:title.data()];
        NSString* msg_str = [NSString stringWithUTF8String:message.data()];
        [alert setMessageText:title_str];
        [alert setInformativeText:msg_str];
        alert.alertStyle = get_style(style);
        msgbox_result res[2];
        switch (buttons)
        {
        case msgbox_buttons::ok_cancel:
            [alert addButtonWithTitle:@"OK"];
            [alert addButtonWithTitle:@"Cancel"];
            res[0] = msgbox_result::ok;
            res[1] = msgbox_result::cancel;
            break;
        case msgbox_buttons::yes_no:
            [alert addButtonWithTitle:@"Yes"];
            [alert addButtonWithTitle:@"No"];
            res[0] = msgbox_result::yes;
            res[0] = msgbox_result::no;
            break;
        default:
            [alert addButtonWithTitle:@"OK"];
            res[0] = msgbox_result::ok;
            break;
        }
        switch ([alert runModal])
        {
        case NSAlertFirstButtonReturn:
            return res[0];
        case NSAlertSecondButtonReturn:
            return res[1];
        default:
            return msgbox_result::error_result;
        }
    }
}
