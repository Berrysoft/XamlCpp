#include <xaml/ui/password_entry.hpp>

namespace xaml
{
    XAML_API password_entry::password_entry() : entry()
    {
        add_password_char_changed([this](password_entry&, char_t) {
            if (get_handle())
            {
                draw_password_char();
                __parent_redraw();
            }
        });
    }

    XAML_API password_entry::~password_entry() {}
} // namespace xaml
