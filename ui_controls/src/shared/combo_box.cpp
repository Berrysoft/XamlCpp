#include <xaml/ui/controls/combo_box.hpp>

namespace xaml
{
    combo_box::combo_box() : items_base()
    {
        add_size_changed([this](control const&, size) {
            if (get_handle())
            {
                draw_size();
                __parent_redraw();
            }
        });
        add_items_changed([this](items_base&, observable_vector_view<string_t>) { if (get_handle()) draw_items(); });
    }

    combo_box::~combo_box() {}
} // namespace xaml
