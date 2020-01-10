#include <xaml/ui/entry.hpp>

namespace xaml
{
    XAML_API entry::entry() : common_control()
    {
        add_text_changed([this](entry const&, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
        add_size_changed([this](control const&, size) {
            if (get_handle())
            {
                draw_size();
                __parent_redraw();
            }
        });
    }

    XAML_API entry::~entry() {}
} // namespace xaml
