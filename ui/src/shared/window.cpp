#include <xaml/ui/window.hpp>

using namespace std;

namespace xaml
{
    window::window() : container(), m_resizable(true)
    {
        add_title_changed([this](shared_ptr<window>, string_view_t) { if (get_handle()) draw_title(); });
        add_location_changed([this](shared_ptr<window>, point) { if (get_handle() && !m_resizing) __draw({}); });
        add_resizable_changed([this](shared_ptr<window>, bool) { if(get_handle()) draw_resizable(); });
    }
} // namespace xaml
