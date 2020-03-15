#include <gtk/gtk.h>
#include <xaml/ui/msgbox.hpp>
#include <xaml/ui/native_control.hpp>

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

    msgbox_result msgbox(shared_ptr<window> parent, string_view_t message, string_view_t title, string_view_t instruction, msgbox_style style, array_view<msgbox_button> buttons)
    {
        GtkWidget* dialog = gtk_message_dialog_new(parent && parent->get_handle() ? GTK_WINDOW(parent->get_handle()->handle) : NULL, GTK_DIALOG_DESTROY_WITH_PARENT, get_style(style), GTK_BUTTONS_NONE, "%s", instruction.empty() ? message.data() : instruction.data());
        if (!title.empty()) gtk_window_set_title(GTK_WINDOW(dialog), title.data());
        if (!instruction.empty()) gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", message.data());
        for (auto& button : buttons)
        {
            switch (button.index())
            {
            case 0:
                switch (get<msgbox_common_button>(button))
                {
                case msgbox_common_button::ok:
                    gtk_dialog_add_button(GTK_DIALOG(dialog), U("_OK"), (gint)msgbox_result::ok);
                    break;
                case msgbox_common_button::yes:
                    gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Yes"), (gint)msgbox_result::yes);
                    break;
                case msgbox_common_button::no:
                    gtk_dialog_add_button(GTK_DIALOG(dialog), U("_No"), (gint)msgbox_result::no);
                    break;
                case msgbox_common_button::cancel:
                    gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Cancel"), (gint)msgbox_result::cancel);
                    break;
                case msgbox_common_button::retry:
                    gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Retry"), (gint)msgbox_result::retry);
                    break;
                case msgbox_common_button::close:
                    gtk_dialog_add_button(GTK_DIALOG(dialog), U("_Close"), (gint)msgbox_result::cancel);
                    break;
                }
                break;
            case 1:
            {
                msgbox_custom_button const& b = get<msgbox_custom_button>(button);
                gtk_dialog_add_button(GTK_DIALOG(dialog), b.text.data(), (gint)b.result);
                break;
            }
            }
        }
        msgbox_result result = (msgbox_result)gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return result;
    }
} // namespace xaml
