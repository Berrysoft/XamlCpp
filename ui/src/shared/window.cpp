#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    window::window() : container(), m_resizable(true)
    {
        add_title_changed([this](window const&, string_view_t) { if (get_handle()) draw_title(); });
        add_location_changed([this](window const&, point) { if (get_handle() && !m_resizing) __draw({}); });
        add_size_changed([this](control const&, size) { if (get_handle() && !m_resizing) __draw({}); });
        add_resizable_changed([this](control const&, bool) { if(get_handle()) draw_resizable(); });
    }

#ifndef XAML_UI_WINDOWS
    point window::__get_real_location() const
    {
        return get_location();
    }

    void window::__set_real_location(point value)
    {
        set_location(value);
    }

    rectangle window::__get_real_client_region() const
    {
        return get_client_region();
    }
#endif // !XAML_UI_WINDOWS
} // namespace xaml
