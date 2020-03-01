#include <xaml/ui/controls/label.hpp>

namespace xaml
{
    label::label() : control()
    {
        add_text_changed([this](label const&, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
    }

    label::~label() {}
} // namespace xaml
