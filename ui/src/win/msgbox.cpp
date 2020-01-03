#include <xaml/ui/msgbox.hpp>

using namespace std;

namespace xaml
{
    static UINT get_style(msgbox_style style)
    {
        switch (style)
        {
        case msgbox_style::info:
            return MB_ICONINFORMATION;
        case msgbox_style::warning:
            return MB_ICONWARNING;
        case msgbox_style::question:
            return MB_ICONQUESTION;
        case msgbox_style::error:
            return MB_ICONERROR;
        default:
            return 0;
        }
    }

    static UINT get_buttons(msgbox_buttons buttons)
    {
        switch (buttons)
        {
        case msgbox_buttons::yes_no:
            return MB_YESNO;
        case msgbox_buttons::ok_cancel:
            return MB_OKCANCEL;
        default:
            return MB_OK;
        }
    }

    static msgbox_result get_result(int result)
    {
        switch (result)
        {
        case IDOK:
            return msgbox_result::ok;
        case IDCANCEL:
            return msgbox_result::cancel;
        case IDYES:
            return msgbox_result::yes;
        case IDNO:
            return msgbox_result::no;
        default:
            return msgbox_result::error_result;
        }
    }

    XAML_API msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        return get_result(MessageBox(parent ? parent->get_handle() : NULL, message.data(), title.data(), get_style(style) | get_buttons(buttons)));
    }
} // namespace xaml
