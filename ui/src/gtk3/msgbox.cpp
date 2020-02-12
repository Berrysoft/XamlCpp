#include <xaml/ui/msgbox.hpp>

using namespace std;

namespace xaml
{
    static GtkMessageType get_style(msgbox_style style)
    {
        switch (style)
        {
        case msgbox_style::info:
            return GTK_MESSAGE_INFO;
        case msgbox_style::warning:
            return GTK_MESSAGE_WARNING;
        case msgbox_style::question:
            return GTK_MESSAGE_QUESTION;
        case msgbox_style::error:
            return GTK_MESSAGE_ERROR;
        default:
            return GTK_MESSAGE_OTHER;
        }
    }

    msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        GtkWidget* dialog = gtk_message_dialog_new(parent ? GTK_WINDOW(parent->get_handle()) : NULL, GTK_DIALOG_DESTROY_WITH_PARENT, get_style(style), GTK_BUTTONS_NONE, "%s", message.data());
        gtk_window_set_title(GTK_WINDOW(dialog), title.data());
        switch (buttons)
        {
        case msgbox_buttons::ok_cancel:
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_OK"), (gint)msgbox_result::ok);
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Cancel"), (gint)msgbox_result::cancel);
            break;
        case msgbox_buttons::abort_retry_ignore:
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Abort"), (gint)msgbox_result::abort);
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Retry"), (gint)msgbox_result::retry);
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Ignore"), (gint)msgbox_result::ignore);
            break;
        case msgbox_buttons::yes_no_cancel:
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Yes"), (gint)msgbox_result::yes);
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_No"), (gint)msgbox_result::no);
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Cancel"), (gint)msgbox_result::cancel);
            break;
        case msgbox_buttons::yes_no:
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Yes"), (gint)msgbox_result::yes);
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_No"), (gint)msgbox_result::no);
            break;
        case msgbox_buttons::retry_cancel:
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Retry"), (gint)msgbox_result::retry);
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Cancel"), (gint)msgbox_result::cancel);
            break;
        case msgbox_buttons::cancel_try_resume:
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Cancel"), (gint)msgbox_result::cancel);
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Try Again"), (gint)msgbox_result::try_again);
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("C_ontinue"), (gint)msgbox_result::resume);
            break;
        default:
            gtk_dialog_add_button(GTK_DIALOG(dialog), U("_OK"), (gint)msgbox_result::ok);
            break;
        }
        msgbox_result result = (msgbox_result)gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return result;
    }
} // namespace xaml
