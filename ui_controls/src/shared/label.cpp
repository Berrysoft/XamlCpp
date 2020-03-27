#include <xaml/ui/controls/label.hpp>

using namespace std;

namespace xaml
{
    label::label() : control()
    {
        add_text_changed([this](shared_ptr<label>, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
    }

    label::~label() {}
} // namespace xaml
