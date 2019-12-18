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

    point control::get_location() const
    {
        GtkAllocation allocation = {};
        gtk_widget_get_allocation(widget, &allocation);
        return { allocation.x, allocation.y };
    }

    void control::set_location(point value)
    {
        if (GTK_IS_WINDOW(widget))
        {
            gtk_window_move(GTK_WINDOW(widget), value.x, value.y);
        }
        else
        {
            gdk_window_move(gtk_widget_get_window(widget), value.x, value.y);
        }
    }

    size control::get_size() const
    {
        GtkAllocation allocation = {};
        gtk_widget_get_allocation(widget, &allocation);
        return { allocation.width, allocation.height };
    }

    void control::set_size(size value)
    {
        gdk_window_resize(gtk_widget_get_window(widget), value.width, value.height);
    }
} // namespace xaml
