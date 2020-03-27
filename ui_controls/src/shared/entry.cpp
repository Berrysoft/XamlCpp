#include <xaml/ui/controls/entry.hpp>

using namespace std;

namespace xaml
{
    entry::entry() : control()
    {
        add_text_changed([this](shared_ptr<entry>, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
    }

    entry::~entry() {}
} // namespace xaml
