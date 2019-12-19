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
                gtk_container_remove(GTK_CONTAINER(_parent->get_handle()), _handle);
            }
            if (value)
            {
                gtk_container_add(GTK_CONTAINER(value->get_handle()), _handle);
            }
            _parent = value;
        }
    }

    control::control() : _handle(nullptr) {}

    control::~control() {}

    point control::get_location() const
    {
        GtkAllocation allocation = {};
        gtk_widget_get_allocation(_handle, &allocation);
        return { allocation.x, allocation.y };
    }

    void control::set_location(point value)
    {
        if (GTK_IS_WINDOW(_handle))
        {
            gtk_window_move(GTK_WINDOW(_handle), value.x, value.y);
        }
        else
        {
            gdk_window_move(gtk_widget_get_window(_handle), value.x, value.y);
        }
    }

    size control::get_size() const
    {
        GtkAllocation allocation = {};
        gtk_widget_get_allocation(_handle, &allocation);
        return { allocation.width, allocation.height };
    }

    void control::set_size(size value)
    {
        gdk_window_resize(gtk_widget_get_window(_handle), value.width, value.height);
    }
} // namespace xaml
