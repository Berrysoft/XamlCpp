#ifndef XAML_UI_MSGBOX_HPP
#define XAML_UI_MSGBOX_HPP

#include <xaml/ui/strings.hpp>
#include <xaml/ui/window.hpp>

namespace xaml
{
    enum class msgbox_style
    {
        none,
        info,
        warning,
        question,
        error
    };

    enum class msgbox_buttons
    {
        ok,
        yes_no,
        ok_cancel
    };

    enum class msgbox_result
    {
        error_result,
        ok,
        cancel,
        yes,
        no
    };

    msgbox_result msgbox(std::shared_ptr<window> parent, string_view_t message, string_view_t title = {}, msgbox_style style = msgbox_style::none, msgbox_buttons buttons = msgbox_buttons::ok);
    inline msgbox_result msgbox(string_view_t message, string_view_t title = {}, msgbox_style style = msgbox_style::none, msgbox_buttons buttons = msgbox_buttons::ok) { return msgbox(nullptr, message, title, style, buttons); }
} // namespace xaml

#endif // !XAML_UI_MSGBOX_HPP
