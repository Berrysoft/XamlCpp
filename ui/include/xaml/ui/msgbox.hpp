#ifndef XAML_UI_MSGBOX_HPP
#define XAML_UI_MSGBOX_HPP

#include <variant>
#include <xaml/strings.hpp>
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
        ok_cancel,
        abort_retry_ignore,
        yes_no_cancel,
        yes_no,
        retry_cancel,
        cancel_try_resume
    };

    enum class msgbox_result
    {
        error_result,
        ok,
        cancel,
        abort,
        retry,
        ignore,
        yes,
        no,
        try_again = 10,
        resume
    };

    XAML_UI_API msgbox_result msgbox(std::shared_ptr<window> parent, string_view_t message, string_view_t title = {}, msgbox_style style = msgbox_style::none, msgbox_buttons buttons = msgbox_buttons::ok);
    inline msgbox_result msgbox(string_view_t message, string_view_t title = {}, msgbox_style style = msgbox_style::none, msgbox_buttons buttons = msgbox_buttons::ok) { return msgbox(nullptr, message, title, style, buttons); }

    XAML_UI_API msgbox_result msgbox_ex(std::shared_ptr<window> parent, string_view_t instruction, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons);

    enum class msgbox_common_button
    {
        ok = 0x1,
        yes = 0x2,
        no = 0x4,
        cancel = 0x8,
        retry = 0x10,
        close = 0x20
    };

    struct msgbox_custom_button
    {
        msgbox_result result;
        string_view_t text;
    };

    using msgbox_button = std::variant<msgbox_common_button, msgbox_custom_button>;

    XAML_UI_API msgbox_result msgbox_ex(std::shared_ptr<window> parent, string_view_t instruction, string_view_t message, string_view_t title, msgbox_style style, array_view<msgbox_button> buttons);

    inline msgbox_result msgbox_ex(std::shared_ptr<window> parent, string_view_t instruction, string_view_t message, string_view_t title = {}, msgbox_style style = msgbox_style::none) { return msgbox_ex(nullptr, instruction, message, title, style, msgbox_buttons::ok); }
} // namespace xaml

#endif // !XAML_UI_MSGBOX_HPP
