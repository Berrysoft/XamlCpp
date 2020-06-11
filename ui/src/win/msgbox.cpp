#include <vector>
#include <xaml/ui/msgbox.h>
#include <xaml/ui/win/control.h>

#include <CommCtrl.h>

using namespace std;

xaml_result XAML_CALL xaml_msgbox_custom(xaml_window* parent, xaml_string* message, xaml_string* title, xaml_string* instruction, xaml_msgbox_style style, xaml_vector_view* buttons, xaml_msgbox_result* presult) noexcept
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

    xaml_codecvt_pool pool;

    {
        wstring_view data;
        XAML_RETURN_IF_FAILED(pool(title, &data));
        config.pszWindowTitle = data.data();
    }
    {
        wstring_view data;
        XAML_RETURN_IF_FAILED(pool(instruction, &data));
        config.pszMainInstruction = data.data();
    }
    {
        wstring_view data;
        XAML_RETURN_IF_FAILED(pool(message, &data));
        config.pszContent = data.data();
    }
    TASKDIALOG_COMMON_BUTTON_FLAGS flags = 0;
    vector<TASKDIALOG_BUTTON> cbs;
    XAML_FOREACH_START(b, buttons);
    {
        xaml_ptr<xaml_box> box;
        XAML_RETURN_IF_FAILED(b->query(&box));
        xaml_msgbox_custom_button const* button;
        XAML_RETURN_IF_FAILED(box->get_value_ptr(&button));
        wstring_view data;
        XAML_RETURN_IF_FAILED(pool(button->text, &data));
        cbs.push_back({ (int)button->result, data.data() });
    }
    XAML_FOREACH_END();
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
    XAML_RETURN_IF_FAILED(TaskDialogIndirect(&config, &result, nullptr, nullptr));
    *presult = (xaml_msgbox_result)result;
    return XAML_S_OK;
}
