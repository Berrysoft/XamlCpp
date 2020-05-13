#ifdef XAML_UI_WEBVIEW_EDGE

#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.Web.Http.h"
#include "winrt/Windows.Web.UI.h"
#include <robuffer.h>
#include <win/hstring.hpp>
#include <win/webview_edge.hpp>

using namespace std;
using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Web::Http;
using namespace winrt::Windows::Web::UI;
using namespace winrt::Windows::Web::UI::Interop;

#define XAML_CATCH_RETURN_WINRT()         \
    catch (winrt::hresult_error const& e) \
    {                                     \
        return e.code();                  \
    }                                     \
    XAML_CATCH_RETURN()

struct ArrayViewBuffer : implements<ArrayViewBuffer, IBuffer, ::Windows::Storage::Streams::IBufferByteAccess>
{
    xaml_ptr<xaml_buffer> m_buffer;

    ArrayViewBuffer(xaml_ptr<xaml_buffer> const& buffer) : m_buffer(buffer) {}

    uint32_t Length() const
    {
        int32_t size;
        check_hresult(m_buffer->get_size(&size));
        return (uint32_t)size;
    }

    uint32_t Capacity() const { return Length(); }

    void Length(uint32_t value) { throw hresult_invalid_argument(); }

    STDMETHODIMP Buffer(uint8_t** value)
    {
        return m_buffer->get_data(value);
    }
};

xaml_result xaml_webview_edge::create_async(HWND parent, xaml_rectangle const& rect, function<xaml_result()>&& callback) noexcept
try
{
    WebViewControlProcess process;
    auto task = process.CreateWebViewControlAsync((int64_t)parent, { (float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height });
    task.Completed([this, callback](IAsyncOperation<WebViewControl> operation, AsyncStatus status) {
        if (status == AsyncStatus::Completed)
        {
            m_view = operation.GetResults();
            m_view.NavigationCompleted([this](IWebViewControl const&, WebViewControlNavigationCompletedEventArgs const& args) {
                check_hresult(invoke_navigated(args.Uri().AbsoluteUri().data()));
            });
            m_view.WebResourceRequested([this](IWebViewControl const&, WebViewControlWebResourceRequestedEventArgs const e) {
                auto deferral = e.GetDeferral();
                auto req = e.Request();
                auto method = req.Method().ToString();
                auto uri = req.RequestUri().AbsoluteUri();
                xaml_ptr<xaml_webview_resource_requested_args> args;
                check_hresult(xaml_webview_resource_requested_args_new(&args));
                xaml_ptr<xaml_webview_web_request> args_req;
                check_hresult(xaml_webview_web_request_new(&args_req));
                check_hresult(args->set_request(args_req.get()));

                xaml_ptr<xaml_string> method_str;
                check_hresult(xaml_string_new_hstring(method, &method_str));
                check_hresult(args_req->set_method(method_str.get()));
                xaml_ptr<xaml_string> uri_str;
                check_hresult(xaml_string_new_hstring(uri, &uri_str));
                check_hresult(args_req->set_uri(uri_str.get()));
                check_hresult(invoke_resource_requested(args));

                xaml_ptr<xaml_webview_web_response> args_res;
                check_hresult(args->get_response(&args_res));
                if (args_res)
                {
                    xaml_ptr<xaml_buffer> b;
                    check_hresult(args_res->get_data(&b));
                    auto buffer = make<ArrayViewBuffer>(b);
                    HttpResponseMessage message;
                    message.Content(HttpBufferContent{ buffer });
                    e.Response(message);
                }
                deferral.Complete();
            });
        }
        check_hresult(callback());
    });
    return XAML_S_OK;
}
XAML_CATCH_RETURN_WINRT()

xaml_result xaml_webview_edge::navigate(xaml_char_t const* uri) noexcept
try
{
    m_view.Navigate(Uri{ uri });
    return XAML_S_OK;
}
XAML_CATCH_RETURN_WINRT()

xaml_result xaml_webview_edge::set_visible(bool vis) noexcept
try
{
    m_view.IsVisible(vis);
    return XAML_S_OK;
}
XAML_CATCH_RETURN_WINRT()

xaml_result xaml_webview_edge::set_location(xaml_point const& p) noexcept
try
{
    auto bounds = m_view.Bounds();
    m_view.Bounds({ (float)p.x, (float)p.y, bounds.Width, bounds.Height });
    return XAML_S_OK;
}
XAML_CATCH_RETURN_WINRT()

xaml_result xaml_webview_edge::set_size(xaml_size const& s) noexcept
try
{
    auto bounds = m_view.Bounds();
    m_view.Bounds({ bounds.X, bounds.Y, (float)s.width, (float)s.height });
    return XAML_S_OK;
}
XAML_CATCH_RETURN_WINRT()

xaml_result xaml_webview_edge::set_rect(xaml_rectangle const& rect) noexcept
try
{
    m_view.Bounds({ (float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height });
    return XAML_S_OK;
}
XAML_CATCH_RETURN_WINRT()

xaml_result xaml_webview_edge::go_forward() noexcept
try
{
    m_view.GoForward();
    return XAML_S_OK;
}
XAML_CATCH_RETURN_WINRT()

xaml_result xaml_webview_edge::go_back() noexcept
try
{
    m_view.GoBack();
    return XAML_S_OK;
}
XAML_CATCH_RETURN_WINRT()

xaml_result xaml_webview_edge::get_can_go_forward(bool* pvalue) noexcept
try
{
    *pvalue = m_view.CanGoForward();
    return XAML_S_OK;
}
XAML_CATCH_RETURN_WINRT()

xaml_result xaml_webview_edge::get_can_go_back(bool* pvalue) noexcept
try
{
    *pvalue = m_view.CanGoBack();
    return XAML_S_OK;
}
XAML_CATCH_RETURN_WINRT()

#endif // XAML_UI_WEBVIEW_EDGE
