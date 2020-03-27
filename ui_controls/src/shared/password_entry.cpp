#include <xaml/ui/controls/password_entry.hpp>

using namespace std;

namespace xaml
{
    password_entry::password_entry() : entry()
    {
        add_password_char_changed([this](shared_ptr<password_entry>, char_t) {
            if (get_handle())
            {
                draw_password_char();
                __parent_redraw();
            }
        });
    }

    password_entry::~password_entry() {}
} // namespace xaml
