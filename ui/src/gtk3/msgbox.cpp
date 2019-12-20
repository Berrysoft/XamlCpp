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

    static GtkButtonsType get_buttons(msgbox_buttons buttons)
    {
        switch (buttons)
        {
        case msgbox_buttons::yes_no:
            return GTK_BUTTONS_YES_NO;
        case msgbox_buttons::ok_cancel:
            return GTK_BUTTONS_OK_CANCEL;
        default:
            return GTK_BUTTONS_OK;
        }
    }

    static msgbox_result get_result(int result)
    {
        switch (result)
        {
        case GTK_RESPONSE_OK:
            return msgbox_result::ok;
        case GTK_RESPONSE_CANCEL:
            return msgbox_result::cancel;
        case GTK_RESPONSE_YES:
            return msgbox_result::yes;
        case GTK_RESPONSE_NO:
            return msgbox_result::no;
        default:
            return msgbox_result::error_result;
        }
    }

    msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        GtkWidget* dialog = gtk_message_dialog_new(parent ? GTK_WINDOW(parent->get_handle()) : NULL, GTK_DIALOG_DESTROY_WITH_PARENT, get_style(style), get_buttons(buttons), "%s", title.data());
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", message.data());
        msgbox_result result = get_result(gtk_dialog_run(GTK_DIALOG(dialog)));
        gtk_widget_destroy(dialog);
        return result;
    }
} // namespace xaml
