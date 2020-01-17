#include <wil/result_macros.h>
#include <windowsx.h>
#include <xaml/ui/password_entry.hpp>

using namespace std;

namespace xaml
{
    static char_t default_char{ U('*') };

    void password_entry::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            window_create_params params = {};
            params.class_name = U("EDIT");
            params.style = WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_LEFT | ES_AUTOHSCROLL | ES_PASSWORD;
            params.ex_style = WS_EX_CLIENTEDGE;
            params.x = 0;
            params.y = 0;
            params.width = 100;
            params.height = 50;
            params.parent = get_parent().get();
            this->__create(params);
            default_char = Edit_GetPasswordChar(get_handle());
        }
        entry::__draw(region);
        draw_password_char();
    }

    void password_entry::draw_password_char()
    {
        Edit_SetPasswordChar(get_handle(), m_password_char ? m_password_char : default_char);
    }
} // namespace xaml
