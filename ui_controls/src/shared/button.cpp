#include <xaml/ui/controls/button.hpp>

using namespace std;

namespace xaml
{
    button::button() : control()
    {
        add_text_changed([this](shared_ptr<button>, string_view_t) {
            if (get_handle())
            {
                draw_text();
                __parent_redraw();
            }
        });
        add_is_default_changed([this](shared_ptr<button>, bool) { if (get_handle()) draw_default(); });
    }

    button::~button() {}

} // namespace xaml
