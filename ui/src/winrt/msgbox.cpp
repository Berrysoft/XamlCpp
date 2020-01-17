#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.UI.Core.h"
#include "winrt/Windows.UI.Xaml.Controls.h"
#include <Windows.h>
#include <xaml/ui/msgbox.hpp>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;

namespace xaml
{
    static fire_and_forget __msgbox_async(function<void(msgbox_result)> callback, shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        ContentDialog dialog{};
        dialog.Title(box_value(title));
        dialog.Content(box_value(message));
        dialog.DefaultButton(ContentDialogButton::Primary);
        msgbox_result res[2];
        switch (buttons)
        {
        case msgbox_buttons::yes_no:
            dialog.IsPrimaryButtonEnabled(true);
            dialog.PrimaryButtonText(L"Yes");
            dialog.CloseButtonText(L"No");
            res[0] = msgbox_result::no;
            res[1] = msgbox_result::yes;
            break;
        case msgbox_buttons::ok_cancel:
            dialog.IsPrimaryButtonEnabled(true);
            dialog.PrimaryButtonText(L"OK");
            dialog.CloseButtonText(L"Cancel");
            res[0] = msgbox_result::cancel;
            res[1] = msgbox_result::ok;
            break;
        default:
            dialog.CloseButtonText(L"OK");
            res[0] = msgbox_result::ok;
            break;
        }
        co_await resume_foreground(parent->get_handle().Dispatcher());
        auto result = co_await dialog.ShowAsync();
        if (callback)
        {
            switch (result)
            {
            case ContentDialogResult::Primary:
                callback(res[1]);
            default:
                callback(res[0]);
            }
        }
    }

    msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        __msgbox_async({}, parent, message, title, style, buttons);
        return msgbox_result::error_result;
    }

    void msgbox_async(shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons, function<void(msgbox_result)> callback)
    {
        __msgbox_async(callback, parent, message, title, style, buttons);
    }
} // namespace xaml
