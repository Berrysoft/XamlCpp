#include <xaml/ui/controls/entry.hpp>

namespace xaml
{
    entry::entry() : control()
    {
        add_text_changed([this](entry const&, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
    }

    entry::~entry() {}
} // namespace xaml
