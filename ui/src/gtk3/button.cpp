#include <xaml/ui/button.hpp>

namespace xaml
{
    button::button() : control()
    {
        set_handle(gtk_button_new());
    }

    button::~button()
    {
        gtk_widget_destroy(get_handle());
    }

    string_t button::get_text() const
    {
        return gtk_button_get_label(GTK_BUTTON(get_handle()));
    }

    void button::set_text(string_view_t value)
    {
        gtk_button_set_label(GTK_BUTTON(get_handle()), value.data());
    }

    bool button::get_is_default() const
    {
        return gtk_button_get_relief(GTK_BUTTON(get_handle())) == GTK_RELIEF_NORMAL;
    }

    void button::set_is_default(bool value)
    {
        gtk_button_set_relief(GTK_BUTTON(get_handle()), value ? GTK_RELIEF_NORMAL : GTK_RELIEF_NONE);
    }
} // namespace xaml
