#include <xaml/ui/password_entry.hpp>

namespace xaml
{
    XAML_API password_entry::password_entry() : common_control()
    {
        add_text_changed([this](password_entry const&, string_view_t) {
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
