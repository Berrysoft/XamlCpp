#ifdef XAML_UI_WEBVIEW_EDGE

#include "winrt/Windows.Foundation.h"
#include "winrt/Windows.Storage.Streams.h"
#include "winrt/Windows.Web.Http.h"
#include "winrt/Windows.Web.UI.h"
#include <robuffer.h>
#include <win/webview_edge.hpp>

using namespace std;
using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Web::Http;
using namespace winrt::Windows::Web::UI;
using namespace winrt::Windows::Web::UI::Interop;

namespace xaml
{
    struct ArrayViewBuffer : implements<ArrayViewBuffer, IBuffer, ::Windows::Storage::Streams::IBufferByteAccess>
    {
        array_view<std::byte> m_buffer;

        ArrayViewBuffer(array_view<std::byte> buffer) : m_buffer(buffer) {}

        uint32_t Capacity() const { return (uint32_t)m_buffer.size(); }

        uint32_t Length() const { return (uint32_t)m_buffer.size(); }

        void Length(uint32_t value) { throw hresult_invalid_argument(); }

        HRESULT __stdcall Buffer(uint8_t** value)
        {
            *value = (uint8_t*)m_buffer.data();
            return S_OK;
        }
    };

    void webview_edge::create_async(HWND parent, rectangle const& rect, function<void()>&& callback)
    {
        try
        {
            WebViewControlProcess process;
            auto task = process.CreateWebViewControlAsync((int64_t)parent, { (float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height });
            task.Completed([this, callback](IAsyncOperation<WebViewControl> operation, AsyncStatus status) {
                if (status == AsyncStatus::Completed)
                {
                    m_view = operation.GetResults();
                    m_view.NavigationCompleted([this](IWebViewControl const&, WebViewControlNavigationCompletedEventArgs const& args) {
                        invoke_navigated(args.Uri().AbsoluteUri());
                    });
                    m_view.WebResourceRequested([this](IWebViewControl const&, WebViewControlWebResourceRequestedEventArgs const e) {
                        auto deferral = e.GetDeferral();
                        auto req = e.Request();
                        auto method = req.Method().ToString();
                        auto uri = req.RequestUri().AbsoluteUri();
                        resource_requested_args args{};
                        args.request.method = method;
                        args.request.uri = uri;
                        invoke_resource_requested(args);
                        if (args.response)
                        {
                            auto& res = *args.response;
                            HttpResponseMessage message;
                            auto buffer = make<ArrayViewBuffer>(res.data);
                            message.Content(HttpBufferContent{ buffer });
                            e.Response(message);
                        }
                        deferral.Complete();
                    });
                }
                callback();
            });
        }
        catch (hresult_error const&)
        {
            m_view = nullptr;
            callback();
        }
    }

    void webview_edge::navigate(string_view_t uri)
    {
        m_view.Navigate(Uri{ uri });
    }

    void webview_edge::set_visible(bool vis)
    {
        m_view.IsVisible(vis);
    }

    void webview_edge::set_location(point p)
    {
        auto bounds = m_view.Bounds();
        m_view.Bounds({ (float)p.x, (float)p.y, bounds.Width, bounds.Height });
    }

    void webview_edge::set_size(size s)
    {
        auto bounds = m_view.Bounds();
        m_view.Bounds({ bounds.X, bounds.Y, (float)s.width, (float)s.height });
    }

    void webview_edge::set_rect(rectangle const& rect)
    {
        m_view.Bounds({ (float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height });
    }

    void webview_edge::go_forward()
    {
        m_view.GoForward();
    }

    void webview_edge::go_back()
    {
        m_view.GoBack();
    }

    bool webview_edge::get_can_go_forward()
    {
        return m_view.CanGoForward();
    }

    bool webview_edge::get_can_go_back()
    {
        return m_view.CanGoBack();
    }
} // namespace xaml

#endif // XAML_UI_WEBVIEW_EDGE
