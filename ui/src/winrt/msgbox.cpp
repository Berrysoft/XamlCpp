#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.UI.Xaml.Controls.h"
#include <Windows.h>
#include <pplawait.h>
#include <ppltasks.h>
#include <xaml/ui/msgbox.hpp>

using namespace std;
using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Controls;
using namespace concurrency;

namespace xaml
{
    static task<msgbox_result> msgbox_async(shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        if (parent)
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
            auto result = co_await dialog.ShowAsync();
            switch (result)
            {
            case ContentDialogResult::Primary:
                co_return res[1];
            default:
                co_return res[0];
            }
        }
        co_return msgbox_result::error_result;
    }

    XAML_API msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        auto t = msgbox_async(parent, message, title, style, buttons);
        t.wait();
        return t.get();
    }
} // namespace xaml
