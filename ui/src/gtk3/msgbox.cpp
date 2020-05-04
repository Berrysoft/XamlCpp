#include <gtk/gtk.h>
#include <xaml/ui/gtk3/window.h>
#include <xaml/ui/msgbox.h>

using namespace std;

static GtkMessageType get_style(xaml_msgbox_style style) noexcept
{
    switch (style)
    {
    case xaml_msgbox_info:
        return GTK_MESSAGE_INFO;
    case xaml_msgbox_warning:
        return GTK_MESSAGE_WARNING;
    case xaml_msgbox_question:
        return GTK_MESSAGE_QUESTION;
    case xaml_msgbox_error:
        return GTK_MESSAGE_ERROR;
    default:
        return GTK_MESSAGE_OTHER;
    }
}

xaml_result XAML_CALL xaml_msgbox_custom(xaml_window* parent, xaml_string* message, xaml_string* title, xaml_string* instruction, xaml_msgbox_style style, xaml_vector_view* buttons, xaml_msgbox_result* presult) noexcept
{
    GtkWidget* parent_handle = nullptr;
    if (parent)
    {
        xaml_ptr<xaml_gtk3_window> native_control;
        if (XAML_SUCCEEDED(parent->query(&native_control)))
        {
            XAML_RETURN_IF_FAILED(native_control->get_window(&parent_handle));
        }
    }
    char const* msg_data;
    if (instruction)
    {
        XAML_RETURN_IF_FAILED(instruction->get_data(&msg_data));
    }
    else
    {
        XAML_RETURN_IF_FAILED(message->get_data(&msg_data));
    }
    GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(parent_handle), GTK_DIALOG_DESTROY_WITH_PARENT, get_style(style), GTK_BUTTONS_NONE, "%s", msg_data);
    if (title)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(title->get_data(&data));
        gtk_window_set_title(GTK_WINDOW(dialog), data);
    }
    if (!instruction)
    {
        char const* data;
        XAML_RETURN_IF_FAILED(message->get_data(&data));
        gtk_message_dialog_format_secondary_text(GTK_MESSAGE_DIALOG(dialog), "%s", data);
    }
    XAML_FOREACH_START(b, buttons);
    {
        xaml_ptr<xaml_box> box;
        XAML_RETURN_IF_FAILED(b->query(&box));
        xaml_msgbox_custom_button const* button;
        XAML_RETURN_IF_FAILED(box->get_value_ptr(button));
        gtk_dialog_add_button(GTK_DIALOG(dialog), button->text, (gint)button->result);
    }
    XAML_FOREACH_END();
    xaml_msgbox_result result = (xaml_msgbox_result)gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    *presult = result;
    return XAML_S_OK;
}
