#include <xaml/ui/msgbox.hpp>

using namespace std;

namespace xaml
{
    static NSAlertStyle get_style(msgbox_style style)
    {
        switch (style)
        {
        case msgbox_style::warning:
        case msgbox_style::error:
            return NSAlertStyleCritical;
        case msgbox_style::info:
            return NSAlertStyleInformational;
        default:
            return NSAlertStyleWarning;
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
        msgbox_result res[3];
        switch (buttons)
        {
        case msgbox_buttons::ok_cancel:
            [alert addButtonWithTitle:@"OK"];
            [alert addButtonWithTitle:@"Cancel"];
            res[0] = msgbox_result::ok;
            res[1] = msgbox_result::cancel;
            break;
        case msgbox_buttons::abort_retry_ignore:
            [alert addButtonWithTitle:@"Abort"];
            [alert addButtonWithTitle:@"Retry"];
            [alert addButtonWithTitle:@"Ignore"];
            res[0] = msgbox_result::abort;
            res[1] = msgbox_result::retry;
            res[2] = msgbox_result::ignore;
            break;
        case msgbox_buttons::yes_no_cancel:
            [alert addButtonWithTitle:@"Yes"];
            [alert addButtonWithTitle:@"No"];
            [alert addButtonWithTitle:@"Cancel"];
            res[0] = msgbox_result::yes;
            res[1] = msgbox_result::no;
            res[2] = msgbox_result::cancel;
            break;
        case msgbox_buttons::yes_no:
            [alert addButtonWithTitle:@"Yes"];
            [alert addButtonWithTitle:@"No"];
            res[0] = msgbox_result::yes;
            res[1] = msgbox_result::no;
            break;
        case msgbox_buttons::retry_cancel:
            [alert addButtonWithTitle:@"Retry"];
            [alert addButtonWithTitle:@"Cancel"];
            res[0] = msgbox_result::retry;
            res[1] = msgbox_result::cancel;
            break;
        case msgbox_buttons::cancel_try_resume:
            [alert addButtonWithTitle:@"Cancel"];
            [alert addButtonWithTitle:@"Try Again"];
            [alert addButtonWithTitle:@"Continue"];
            res[0] = msgbox_result::cancel;
            res[1] = msgbox_result::try_again;
            res[2] = msgbox_result::resume;
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
        case NSAlertThirdButtonReturn:
            return res[2];
        default:
            return msgbox_result::error_result;
        }
    }
}
