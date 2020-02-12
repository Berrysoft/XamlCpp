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

    msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        return (msgbox_result)MessageBox(parent ? parent->get_handle() : NULL, message.data(), title.data(), get_style(style) | (UINT)buttons);
    }
} // namespace xaml
