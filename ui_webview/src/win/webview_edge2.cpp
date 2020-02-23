#include <Shlwapi.h>
#include <wil/result_macros.h>
#include <win/webview_edge2.hpp>
#include <wrl/event.h>

using namespace std;
using namespace Microsoft::WRL;

namespace xaml
{
    void webview_edge2::create_async(HWND parent, rectangle const& rect, function<void()>&& callback)
    {
        try
        {
            THROW_IF_FAILED(CreateWebView2Environment(
                Callback<IWebView2CreateWebView2EnvironmentCompletedHandler>(
                    [=](HRESULT result, IWebView2Environment* env) -> HRESULT {
                        try
                        {
                            THROW_IF_FAILED(result);
                            m_env = env;
                            THROW_IF_FAILED(env->CreateWebView(
                                parent,
                                Callback<IWebView2CreateWebViewCompletedHandler>(
                                    [=](HRESULT result, IWebView2WebView* webview) -> HRESULT {
                                        try
                                        {
                                            THROW_IF_FAILED(result);
                                            m_view = webview;
                                            THROW_IF_FAILED(m_view->put_Bounds(to_native<RECT>(rect)));
                                            EventRegistrationToken token;
                                            THROW_IF_FAILED(m_view->add_NavigationCompleted(
                                                Callback<IWebView2NavigationCompletedEventHandler>(
                                                    [this](IWebView2WebView*, IWebView2NavigationCompletedEventArgs*) -> HRESULT {
                                                        try
                                                        {
                                                            wil::unique_cotaskmem_string uri;
                                                            THROW_IF_FAILED(m_view->get_Source(&uri));
                                                            invoke_navigated(uri.get());
                                                            return S_OK;
                                                        }
                                                        CATCH_RETURN();
                                                    })
                                                    .Get(),
                                                &token));
                                            wil::com_ptr<IWebView2WebView5> view5;
                                            THROW_IF_FAILED(m_view->QueryInterface(&view5));
                                            THROW_IF_FAILED(view5->add_WebResourceRequested(
                                                Callback<IWebView2WebResourceRequestedEventHandler>(
                                                    [this](IWebView2WebView*, IWebView2WebResourceRequestedEventArgs* e) -> HRESULT {
                                                        try
                                                        {
                                                            resource_requested_args args{};
                                                            wil::com_ptr<IWebView2WebResourceRequest> req;
                                                            THROW_IF_FAILED(e->get_Request(&req));

                                                            wil::unique_cotaskmem_string method;
                                                            THROW_IF_FAILED(req->get_Method(&method));
                                                            args.request.method = method.get();

                                                            wil::unique_cotaskmem_string uri;
                                                            THROW_IF_FAILED(req->get_Uri(&uri));
                                                            args.request.uri = uri.get();

                                                            wil::com_ptr<IStream> stream;
                                                            THROW_IF_FAILED(req->get_Content(&stream));
                                                            auto count = wil::stream_size(stream.get());
                                                            vector<std::byte> data(count);
                                                            THROW_IF_FAILED(stream->Read(data.data(), (ULONG)count, NULL));
                                                            args.request.data = data;

                                                            invoke_resource_requested(args);
                                                            if (args.response)
                                                            {
                                                                auto& res = *args.response;
                                                                wil::com_ptr<IStream> res_data = SHCreateMemStream((const BYTE*)res.data.data(), (UINT)res.data.size());
                                                                wil::com_ptr<IWebView2WebResourceResponse> response;
                                                                THROW_IF_FAILED(m_env->CreateWebResourceResponse(res_data.get(), 200, L"OK", (L"Content-Type: " + (string_t)res.content_type).c_str(), &response));
                                                                THROW_IF_FAILED(e->put_Response(response.get()));
                                                            }
                                                            return S_OK;
                                                        }
                                                        CATCH_RETURN();
                                                    })
                                                    .Get(),
                                                &token));
                                            callback();
                                            return S_OK;
                                        }
                                        CATCH_RETURN();
                                    })
                                    .Get()));
                            return S_OK;
                        }
                        CATCH_RETURN();
                    })
                    .Get()));
        }
        catch (wil::ResultException const&)
        {
            m_view = nullptr;
            callback();
        }
    }

    void webview_edge2::navigate(string_view_t uri)
    {
        THROW_IF_FAILED(m_view->Navigate(uri.data()));
    }

    void webview_edge2::set_location(point p)
    {
        RECT bounds;
        THROW_IF_FAILED(m_view->get_Bounds(&bounds));
        THROW_IF_FAILED(m_view->put_Bounds({ (LONG)p.x, (LONG)p.y, bounds.right, bounds.bottom }));
    }

    void webview_edge2::set_size(size s)
    {
        RECT bounds;
        THROW_IF_FAILED(m_view->get_Bounds(&bounds));
        THROW_IF_FAILED(m_view->put_Bounds({ bounds.left, bounds.top, bounds.left + (LONG)s.width, bounds.top + (LONG)s.height }));
    }

    void webview_edge2::set_rect(rectangle const& rect)
    {
        THROW_IF_FAILED(m_view->put_Bounds(to_native<RECT>(rect)));
    }
} // namespace xaml
