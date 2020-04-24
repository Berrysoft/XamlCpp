#include <shared/check_box.hpp>
#include <xaml/ui/controls/check_box.h>

using namespace std;

xaml_result xaml_check_box_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_button_implement::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_checked_changed));

    std::size_t token;
    XAML_RETURN_IF_FAILED((m_is_checked_changed->add<void, xaml_ptr<xaml_check_box>, bool>(
        [this](xaml_ptr<xaml_check_box>, bool) {
            if (m_handle) XAML_THROW_IF_FAILED(draw_checked());
        },
        &token)));

#ifdef XAML_UI_COCOA
    XAML_RETURN_IF_FAILED((m_click->add<void, xaml_ptr<xaml_check_box>>(
        [this](xaml_ptr<xaml_check_box>) {
            // TODO
        },
        &token)));
#endif // XAML_UI_COCOA
}
