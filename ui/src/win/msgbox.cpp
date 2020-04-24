#include <vector>
#include <wil/result_macros.h>
#include <xaml/ui/msgbox.h>
#include <xaml/ui/win/control.h>

#include <CommCtrl.h>

using namespace std;

xaml_result xaml_msgbox_custom(xaml_window* parent, xaml_string* message, xaml_string* title, xaml_string* instruction, xaml_msgbox_style style, xaml_vector_view* buttons, xaml_msgbox_result* presult) noexcept
{
    TASKDIALOGCONFIG config{};
    config.cbSize = sizeof(TASKDIALOGCONFIG);
    if (parent)
    {
        xaml_ptr<xaml_win32_control> native_control;
        if (XAML_SUCCEEDED(parent->query(&native_control)))
        {
            HWND handle;
            XAML_RETURN_IF_FAILED(native_control->get_handle(&handle));
            config.hwndParent = handle;
        }
    }
    config.dwFlags = TDF_ALLOW_DIALOG_CANCELLATION | TDF_SIZE_TO_CONTENT;
    if (title)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(title->get_data(&data));
        config.pszWindowTitle = data;
    }
    if (instruction)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(instruction->get_data(&data));
        config.pszMainInstruction = data;
    }
    if (message)
    {
        xaml_char_t const* data;
        XAML_RETURN_IF_FAILED(message->get_data(&data));
        config.pszContent = data;
    }
    TASKDIALOG_COMMON_BUTTON_FLAGS flags = 0;
    vector<TASKDIALOG_BUTTON> cbs;
    for (auto b : buttons)
    {
        xaml_ptr<xaml_box> box;
        XAML_RETURN_IF_FAILED(b->query(&box));
        xaml_msgbox_custom_button const* button;
        XAML_RETURN_IF_FAILED(box->get_value_ptr(button));
        cbs.push_back({ (int)button->result, button->text });
    }
    config.dwCommonButtons = flags;
    config.cButtons = (UINT)cbs.size();
    config.pButtons = cbs.data();
    switch (style)
    {
    case xaml_msgbox_info:
    case xaml_msgbox_question:
        config.pszMainIcon = TD_INFORMATION_ICON;
        break;
    case xaml_msgbox_warning:
        config.pszMainIcon = TD_WARNING_ICON;
        break;
    case xaml_msgbox_error:
        config.pszMainIcon = TD_ERROR_ICON;
        break;
    default:
        config.pszMainIcon = nullptr;
        break;
    }
    int result;
    THROW_IF_FAILED(TaskDialogIndirect(&config, &result, nullptr, nullptr));
    return (xaml_msgbox_result)result;
}
