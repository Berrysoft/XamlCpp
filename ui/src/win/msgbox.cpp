#include <vector>
#include <wil/result_macros.h>
#include <xaml/ui/msgbox.hpp>

#include <CommCtrl.h>

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

    msgbox_result msgbox_ex(shared_ptr<window> parent, string_view_t instruction, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
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
        return msgbox_ex(parent, instruction, message, title, style, mbs);
    }

    msgbox_result msgbox_ex(shared_ptr<window> parent, string_view_t instruction, string_view_t message, string_view_t title, msgbox_style style, array_view<msgbox_button> buttons)
    {
        TASKDIALOGCONFIG config{};
        config.cbSize = sizeof(TASKDIALOGCONFIG);
        if (parent) config.hwndParent = parent->get_handle();
        config.pszWindowTitle = title.data();
        config.pszMainInstruction = instruction.data();
        config.pszContent = message.data();
        TASKDIALOG_COMMON_BUTTON_FLAGS flags = 0;
        vector<TASKDIALOG_BUTTON> cbs;
        for (auto& button : buttons)
        {
            switch (button.index())
            {
            case 0:
                flags |= (TASKDIALOG_COMMON_BUTTON_FLAGS)get<msgbox_common_button>(button);
                break;
            case 1:
            {
                msgbox_custom_button const& b = get<msgbox_custom_button>(button);
                cbs.push_back({ (int)b.result, b.text.data() });
                break;
            }
            }
        }
        config.dwCommonButtons = flags;
        config.cButtons = (UINT)cbs.size();
        config.pButtons = cbs.data();
        switch (style)
        {
        case msgbox_style::info:
        case msgbox_style::question:
            config.pszMainIcon = MAKEINTRESOURCE(TD_INFORMATION_ICON);
            break;
        case msgbox_style::warning:
            config.pszMainIcon = MAKEINTRESOURCE(TD_WARNING_ICON);
            break;
        case msgbox_style::error:
            config.pszMainIcon = MAKEINTRESOURCE(TD_ERROR_ICON);
            break;
        }
        int result;
        THROW_IF_FAILED(TaskDialogIndirect(&config, &result, nullptr, nullptr));
        return (msgbox_result)result;
    }
} // namespace xaml
