#include <xaml/ui/msgbox.h>

using namespace std;

xaml_result xaml_msgbox(xaml_window* parent, xaml_string* message, xaml_string* title, xaml_string* instruction, xaml_msgbox_style style, xaml_msgbox_buttons buttons, xaml_msgbox_result* presult) noexcept
{
    vector<xaml_ptr<xaml_object>> mbs;
    switch (buttons)
    {
    case xaml_msgbox_buttons_ok_cancel:
        mbs = { xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_ok, U("OK") }), xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_cancel, U("Cancel") }) };
        break;
    case xaml_msgbox_buttons_abort_retry_ignore:
        mbs = { xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_abort, U("Abort") }), xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_retry, U("Retry") }), xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_ignore, U("Ignore") }) };
        break;
    case xaml_msgbox_buttons_yes_no_cancel:
        mbs = { xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_yes, U("Yes") }), xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_no, U("No") }), xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_cancel, U("Cancel") }) };
        break;
    case xaml_msgbox_buttons_yes_no:
        mbs = { xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_yes, U("Yes") }), xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_no, U("No") }) };
        break;
    case xaml_msgbox_buttons_retry_cancel:
        mbs = { xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_retry, U("Retry") }), xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_cancel, U("Cancel") }) };
        break;
    case xaml_msgbox_buttons_cancel_try_resume:
        mbs = { xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_cancel, U("Cancel") }), xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_try_again, U("Try Again") }), xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_resume, U("Continue") }) };
        break;
    default:
        mbs = { xaml_box_value<xaml_msgbox_custom_button>({ xaml_msgbox_result_ok, U("OK") }) };
        break;
    }
    xaml_ptr<xaml_vector> cbs;
    XAML_RETURN_IF_FAILED(xaml_vector_new(move(mbs), &cbs));
    return xaml_msgbox_custom(parent, message, title, instruction, style, cbs.get(), presult);
}
