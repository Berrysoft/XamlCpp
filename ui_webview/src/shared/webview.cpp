#include <shared/webview.hpp>
#include <xaml/ui/controls/webview.h>

using namespace std;

xaml_result xaml_webview_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_uri_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_resource_requested));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_uri_changed->add_noexcept<void, xaml_ptr<xaml_webview>, xaml_ptr<xaml_string>>(
        [this](xaml_ptr<xaml_webview>, xaml_ptr<xaml_string>) -> xaml_result {
            if (m_handle && !m_navigating)
            {
                XAML_RETURN_IF_FAILED(draw_uri());
            }
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}
