#ifndef XAML_UI_MSGBOX_H
#define XAML_UI_MSGBOX_H

#include <xaml/ui/window.h>

typedef enum xaml_msgbox_style
{
    xaml_msgbox_none,
    xaml_msgbox_info,
    xaml_msgbox_warning,
    xaml_msgbox_question,
    xaml_msgbox_error
} xaml_msgbox_style;

typedef enum xaml_msgbox_buttons
{
    xaml_msgbox_buttons_ok,
    xaml_msgbox_buttons_ok_cancel,
    xaml_msgbox_buttons_abort_retry_ignore,
    xaml_msgbox_buttons_yes_no_cancel,
    xaml_msgbox_buttons_yes_no,
    xaml_msgbox_buttons_retry_cancel,
    xaml_msgbox_buttons_cancel_try_resume
} xaml_msgbox_buttons;

typedef enum xaml_msgbox_result
{
    xaml_msgbox_result_error,
    xaml_msgbox_result_ok,
    xaml_msgbox_result_cancel,
    xaml_msgbox_result_abort,
    xaml_msgbox_result_retry,
    xaml_msgbox_result_ignore,
    xaml_msgbox_result_yes,
    xaml_msgbox_result_no,
    xaml_msgbox_result_try_again = 10,
    xaml_msgbox_result_resume
} xaml_msgbox_result;

typedef struct xaml_msgbox_custom_button xaml_msgbox_custom_button;

struct xaml_msgbox_custom_button
{
    xaml_msgbox_result result;
    char const* text;
};

XAML_TYPE(xaml_msgbox_custom_button, { 0x519f6660, 0x380e, 0x44d4, { 0x98, 0xc1, 0x3d, 0x40, 0xa1, 0xd7, 0x9e, 0xa6 } })

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_msgbox(xaml_window*, xaml_string*, xaml_string*, xaml_string*, xaml_msgbox_style, xaml_msgbox_buttons, xaml_msgbox_result*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_msgbox_custom(xaml_window*, xaml_string*, xaml_string*, xaml_string*, xaml_msgbox_style, xaml_vector_view*, xaml_msgbox_result*) XAML_NOEXCEPT;

#endif // !XAML_UI_MSGBOX_H
