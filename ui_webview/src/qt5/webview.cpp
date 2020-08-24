#include <qt5/qstring.hpp>
#include <shared/webview.hpp>

#ifdef XAML_UI_WEBVIEW_WEBKIT
    #include <QWebView>

using q_webview_t = QWebView;
#else
    #include <QWebEngineView>

using q_webview_t = QWebEngineView;
#endif // XAML_UI_WEBVIEW_WEBKIT

using namespace std;

xaml_result xaml_webview_internal::draw(const xaml_rectangle& region) noexcept
{
    if (!m_handle)
    {
        XAML_RETURN_IF_FAILED(create<q_webview_t>());
        auto webview = static_cast<q_webview_t*>(m_handle);
        QObject::connect(
            webview, &q_webview_t::urlChanged,
            xaml_mem_fn(&xaml_webview_internal::on_url_changed, this));
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_uri());
    }
    return set_rect(region);
}

xaml_result xaml_webview_internal::draw_uri() noexcept
{
    if (auto webview = qobject_cast<q_webview_t*>(m_handle))
    {
        QString uri;
        XAML_RETURN_IF_FAILED(to_QString(m_uri, &uri));
        QUrl url{ uri };
        if (url != webview->url())
        {
            webview->setUrl(url);
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::go_forward() noexcept
{
    if (auto webview = qobject_cast<q_webview_t*>(m_handle))
    {
        webview->forward();
    }
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::go_back() noexcept
{
    if (auto webview = qobject_cast<q_webview_t*>(m_handle))
    {
        webview->back();
    }
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::get_can_go_forward(bool* pvalue) noexcept
{
    *pvalue = true;
    return XAML_S_OK;
}

xaml_result xaml_webview_internal::get_can_go_back(bool* pvalue) noexcept
{
    *pvalue = true;
    return XAML_S_OK;
}

void xaml_webview_internal::on_url_changed(const QUrl& url) noexcept
{
    if (auto webview = qobject_cast<q_webview_t*>(m_handle))
    {
        xaml_ptr<xaml_string> uri;
        XAML_ASSERT_SUCCEEDED(xaml_string_new(url.toString(), &uri));
        XAML_ASSERT_SUCCEEDED(set_uri(uri));
    }
}
