#include <xaml/ui/controls/button.hpp>

namespace xaml
{
    button::button() : control()
    {
        add_text_changed([this](button const&, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
        add_is_default_changed([this](button const&, bool) { if (get_handle()) draw_default(); });
    }

    button::~button() {}

} // namespace xaml
