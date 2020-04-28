#include <shared/password_entry.hpp>
#include <xaml/ui/controls/password_entry.h>

using namespace std;

xaml_result xaml_password_entry_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_password_char_changed));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_password_char_changed->add_noexcept<xaml_ptr<xaml_password_entry>, xaml_char_t>(
        [this](xaml_ptr<xaml_password_entry>, xaml_char_t) -> xaml_result {
            if (m_handle)
            {
                XAML_RETURN_IF_FAILED(draw_password_char());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));
}
