#include <xaml/ui/label.hpp>

namespace xaml
{
    XAML_API label::label() : common_control()
    {
        add_text_changed([this](label const&, string_view_t) {
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

    XAML_API label::~label() {}
} // namespace xaml
