#include <shared/atomic_guard.hpp>
#include <shared/webview.hpp>
#include <xaml/ui/controls/webview.h>

using namespace std;

xaml_result xaml_webview_internal::draw(const xaml_rectangle& region) noexcept
{
    if (!m_handle)
    {
        m_handle = webkit_web_view_new();
        g_signal_connect(G_OBJECT(m_handle), "load-changed", G_CALLBACK(xaml_webview_internal::on_load_changed), this);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_uri());
    }
    return set_rect(region);
}

xaml_result xaml_webview_internal::draw_uri() noexcept
{
    char const* data = nullptr;
    if (m_uri)
    {
        XAML_RETURN_IF_FAILED(m_uri->get_data(&data));
    }
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(m_handle), data);
    return XAML_S_OK;
}

static xaml_result xaml_webview_internal_on_load_changed(WebKitWebView*, WebKitLoadEvent load_event, xaml_webview_internal* self) noexcept
{
    if (load_event == WEBKIT_LOAD_COMMITTED)
    {
        xaml_atomic_guard guard(self->m_navigating);
        if (!guard.test_and_set())
        {
            xaml_ptr<xaml_string> uri;
            XAML_RETURN_IF_FAILED(xaml_string_new(webkit_web_view_get_uri(WEBKIT_WEB_VIEW(self->m_handle)), &uri));
            XAML_RETURN_IF_FAILED(self->set_uri(uri.get()));
        }
    }
    return XAML_S_OK;
}

void xaml_webview_internal::on_load_changed(WebKitWebView* web_view, WebKitLoadEvent load_event, xaml_webview_internal* self) noexcept
{
    XAML_ASSERT_SUCCEEDED(xaml_webview_internal_on_load_changed(web_view, load_event, self));
}

xaml_result xaml_webview_internal::go_forward() noexcept
{
    webkit_web_view_go_forward(WEBKIT_WEB_VIEW(m_handle));
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::go_back() noexcept
{
    webkit_web_view_go_back(WEBKIT_WEB_VIEW(m_handle));
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::get_can_go_forward(bool* pvalue) noexcept
{
    *pvalue = webkit_web_view_can_go_forward(WEBKIT_WEB_VIEW(m_handle));
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::get_can_go_back(bool* pvalue) noexcept
{
    *pvalue = webkit_web_view_can_go_back(WEBKIT_WEB_VIEW(m_handle));
    return XAML_S_OK;
}
