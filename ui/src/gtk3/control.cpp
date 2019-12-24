#include <xaml/ui/container.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    control::~control()
    {
        if (get_handle()) gtk_widget_destroy(get_handle());
    }
} // namespace xaml