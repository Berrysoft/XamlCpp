#include <vector>
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
        return msgbox(parent, message, title, {}, style, buttons);
    }

    msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, string_view_t instruction, msgbox_style style, array_view<msgbox_button> buttons)
    {
        NSAlert* alert = [NSAlert new];
        if (instruction.empty())
            [alert setMessageText:[NSString stringWithUTF8String:title.data()]];
        else
            [alert setMessageText:[NSString stringWithUTF8String:instruction.data()]];
        [alert setInformativeText:[NSString stringWithUTF8String:message.data()]];
        alert.alertStyle = get_style(style);
        vector<msgbox_result> res;
        for (auto& button : buttons)
        {
            switch (button.index())
            {
            case 0:
                switch (get<msgbox_common_button>(button))
                {
                case msgbox_common_button::ok:
                    [alert addButtonWithTitle:@"OK"];
                    res.push_back(msgbox_result::ok);
                    break;
                case msgbox_common_button::yes:
                    [alert addButtonWithTitle:@"Yes"];
                    res.push_back(msgbox_result::yes);
                    break;
                case msgbox_common_button::no:
                    [alert addButtonWithTitle:@"No"];
                    res.push_back(msgbox_result::no);
                    break;
                case msgbox_common_button::cancel:
                    [alert addButtonWithTitle:@"Cancel"];
                    res.push_back(msgbox_result::cancel);
                    break;
                case msgbox_common_button::retry:
                    [alert addButtonWithTitle:@"Retry"];
                    res.push_back(msgbox_result::retry);
                    break;
                case msgbox_common_button::close:
                    [alert addButtonWithTitle:@"Close"];
                    res.push_back(msgbox_result::cancel);
                    break;
                }
                break;
            case 1:
            {
                msgbox_custom_button const& b = get<msgbox_custom_button>(button);
                [alert addButtonWithTitle:[NSString stringWithUTF8String:b.text.data()]];
                res.push_back(b.result);
                break;
            }
            }
        }
        auto ret = (ptrdiff_t)[alert runModal] - (ptrdiff_t)NSAlertFirstButtonReturn;
        if (ret < 0 || ret >= (ptrdiff_t)res.size())
            return msgbox_result::error_result;
        else
            return res[ret];
    }
}
