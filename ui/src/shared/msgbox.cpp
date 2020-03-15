#include <xaml/ui/msgbox.hpp>

using namespace std;

namespace xaml
{
    msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        return msgbox(parent, message, title, {}, style, buttons);
    }

    msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, string_view_t instruction, msgbox_style style, msgbox_buttons buttons)
    {
        vector<msgbox_button> mbs;
        switch (buttons)
        {
        case msgbox_buttons::ok_cancel:
            mbs = { msgbox_common_button::ok, msgbox_common_button::cancel };
            break;
        case msgbox_buttons::abort_retry_ignore:
            mbs = { msgbox_custom_button{ msgbox_result::abort, U("Abort") }, msgbox_custom_button{ msgbox_result::retry, U("Retry") }, msgbox_custom_button{ msgbox_result::ignore, U("Ignore") } };
            break;
        case msgbox_buttons::yes_no_cancel:
            mbs = { msgbox_common_button::yes, msgbox_common_button::no, msgbox_common_button::cancel };
            break;
        case msgbox_buttons::yes_no:
            mbs = { msgbox_common_button::yes, msgbox_common_button::no };
            break;
        case msgbox_buttons::retry_cancel:
            mbs = { msgbox_common_button::retry, msgbox_common_button::cancel };
            break;
        case msgbox_buttons::cancel_try_resume:
            mbs = { msgbox_custom_button{ msgbox_result::cancel, U("Cancel") }, msgbox_custom_button{ msgbox_result::try_again, U("Try Again") }, msgbox_custom_button{ msgbox_result::resume, U("Continue") } };
            break;
        default:
            mbs = { msgbox_common_button::ok };
            break;
        }
        return msgbox(parent, message, title, instruction, style, mbs);
    }
} // namespace xaml
