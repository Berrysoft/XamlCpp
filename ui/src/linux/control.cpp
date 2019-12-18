#include <xaml/ui/container.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    void control::set_parent(shared_ptr<container> const& value)
    {
        if (_parent != value)
        {
            if (_parent)
            {
                gtk_container_remove(GTK_CONTAINER(_parent->get_widget()), widget);
            }
            if (value)
            {
                gtk_container_add(GTK_CONTAINER(value->get_widget()), widget);
            }
            _parent = value;
        }
    }

    control::control() : widget(nullptr) {}

    control::~control() {}

    string_t control::get_text() const
    {
        if (GTK_IS_WINDOW(widget))
        {
            return gtk_window_get_title(GTK_WINDOW(widget));
        }
        return {};
    }

    void control::set_text(string_view_t value)
    {
        if (GTK_IS_WINDOW(widget))
        {
            gtk_window_set_title(GTK_WINDOW(widget), value.data());
        }
    }
} // namespace xaml
