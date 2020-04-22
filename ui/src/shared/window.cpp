#include <shared/window.hpp>
#include <xaml/ui/window.h>

using namespace std;

xaml_window_impl::xaml_window_impl() : xaml_container_implement()
{
    XAML_THROW_IF_FAILED(xaml_event_new(&m_is_resizable_changed));
    XAML_THROW_IF_FAILED(xaml_event_new(&m_location_changed));
    XAML_THROW_IF_FAILED(xaml_event_new(&m_title_changed));
    XAML_THROW_IF_FAILED(xaml_event_new(&m_closing));

    xaml_ptr<xaml_delegate> title_changed_callback;
    XAML_THROW_IF_FAILED((xaml_delegate_new<void, xaml_ptr<xaml_window>, xaml_ptr<xaml_string>>(
        [this](xaml_ptr<xaml_window>, xaml_ptr<xaml_string>) {
            if (m_handle)
            {
                draw_title();
            }
        },
        &title_changed_callback)));
    size_t token;
    XAML_THROW_IF_FAILED(m_size_changed->add(title_changed_callback.get(), &token));
}

//namespace xaml
//{
//    window::window() : container(), m_resizable(true)
//    {
//        add_title_changed([this](shared_ptr<window>, string_view_t) { if (get_handle()) draw_title(); });
//        add_location_changed([this](shared_ptr<window>, point) { if (get_handle() && !m_resizing) __draw({}); });
//        add_resizable_changed([this](shared_ptr<window>, bool) { if(get_handle()) draw_resizable(); });
//    }
//} // namespace xaml
