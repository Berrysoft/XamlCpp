#include <shared/window.hpp>
#include <xaml/ui/window.h>

using namespace std;

xaml_window_impl::xaml_window_impl() : xaml_container_implement()
{
    XAML_THROW_IF_FAILED(xaml_event_new(&m_is_resizable_changed));
    XAML_THROW_IF_FAILED(xaml_event_new(&m_location_changed));
    XAML_THROW_IF_FAILED(xaml_event_new(&m_title_changed));
    XAML_THROW_IF_FAILED(xaml_event_new(&m_closing));

    size_t token;
    XAML_THROW_IF_FAILED((m_size_changed->add<void, xaml_ptr<xaml_window>, xaml_ptr<xaml_string>>(
        [this](xaml_ptr<xaml_window>, xaml_ptr<xaml_string>) {
            if (m_handle) draw_title();
        },
        &token)));
    XAML_THROW_IF_FAILED((m_location_changed->add<void, xaml_ptr<xaml_window>, xaml_point>(
        [this](xaml_ptr<xaml_window>, xaml_point) {
            if (m_handle && !m_resizing) draw({});
        },
        &token)));
    XAML_THROW_IF_FAILED((m_is_resizable_changed->add<void, xaml_ptr<xaml_window>, bool>(
        [this](xaml_ptr<xaml_window>, bool) {
            if (m_handle) draw_resizable();
        },
        &token)));
}
