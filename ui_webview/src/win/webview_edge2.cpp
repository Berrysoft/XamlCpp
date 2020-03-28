#ifdef XAML_UI_WEBVIEW_WEBVIEW2

#include <Shlwapi.h>
#include <wil/result_macros.h>
#include <win/webview_edge2.hpp>
#include <wrl/event.h>
#include <xaml/ui/native_drawing.hpp>

using namespace std;
using namespace Microsoft::WRL;

namespace xaml
{
    void webview_edge2::create_async(HWND parent, rectangle const& rect, function<void()>&& callback)
    {
        HRESULT hr = CreateCoreWebView2Environment(
            Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                [=](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                    RETURN_IF_FAILED(result);
                    m_env = env;
                    RETURN_IF_FAILED(env->CreateCoreWebView2Host(
                        parent,
                        Callback<ICoreWebView2CreateCoreWebView2HostCompletedHandler>(
                            [=](HRESULT result, ICoreWebView2Host* webview) -> HRESULT {
                                RETURN_IF_FAILED(result);
                                m_host = webview;
                                RETURN_IF_FAILED(m_host->put_Bounds(to_native<RECT>(rect)));
                                RETURN_IF_FAILED(m_host->get_CoreWebView2(&m_view));
                                EventRegistrationToken token;
                                RETURN_IF_FAILED(m_view->add_NavigationCompleted(
                                    Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                        [this](ICoreWebView2*, ICoreWebView2NavigationCompletedEventArgs*) -> HRESULT {
                                            wil::unique_cotaskmem_string uri;
                                            RETURN_IF_FAILED(m_view->get_Source(&uri));
                                            invoke_navigated(uri.get());
                                            return S_OK;
                                        })
                                        .Get(),
                                    &token));
                                RETURN_IF_FAILED(m_view->AddWebResourceRequestedFilter(L"*", CORE_WEBVIEW2_WEB_RESOURCE_CONTEXT_ALL));
                                RETURN_IF_FAILED(m_view->add_WebResourceRequested(
                                    Callback<ICoreWebView2WebResourceRequestedEventHandler>(
                                        [this](ICoreWebView2*, ICoreWebView2WebResourceRequestedEventArgs* e) -> HRESULT {
                                            auto args = make_shared<resource_requested_args>();
                                            wil::com_ptr<ICoreWebView2WebResourceRequest> req;
                                            RETURN_IF_FAILED(e->get_Request(&req));

                                            wil::unique_cotaskmem_string method;
                                            RETURN_IF_FAILED(req->get_Method(&method));
                                            args->request.method = method.get();

                                            wil::unique_cotaskmem_string uri;
                                            RETURN_IF_FAILED(req->get_Uri(&uri));
                                            args->request.uri = uri.get();

                                            wil::com_ptr<IStream> stream;
                                            RETURN_IF_FAILED(req->get_Content(&stream));
                                            if (stream)
                                            {
                                                auto count = wil::stream_size(stream.get());
                                                vector<std::byte> data((size_t)count);
                                                RETURN_IF_FAILED(stream->Read(data.data(), (ULONG)count, NULL));
                                                args->request.data = data;
                                            }

                                            invoke_resource_requested(args);
                                            if (args->response)
                                            {
                                                auto& res = *args->response;
                                                wil::com_ptr<IStream> res_data = SHCreateMemStream((const BYTE*)res.data.data(), (UINT)res.data.size());
                                                wil::com_ptr<ICoreWebView2WebResourceResponse> response;
                                                RETURN_IF_FAILED(m_env->CreateWebResourceResponse(res_data.get(), 200, L"OK", (L"Content-Type: " + (string_t)res.content_type).c_str(), &response));
                                                RETURN_IF_FAILED(e->put_Response(response.get()));
                                            }
                                            return S_OK;
                                        })
                                        .Get(),
                                    &token));
                                callback();
                                return S_OK;
                            })
                            .Get()));
                    return S_OK;
                })
                .Get());
        if (FAILED(hr))
        {
            m_view = nullptr;
            callback();
        }
    }

    void webview_edge2::navigate(string_view_t uri)
    {
        THROW_IF_FAILED(m_view->Navigate(uri.data()));
    }

    void webview_edge2::set_visible(bool vis)
    {
        THROW_IF_FAILED(m_host->put_IsVisible(vis));
    }

    void webview_edge2::set_location(point p)
    {
        RECT bounds;
        THROW_IF_FAILED(m_host->get_Bounds(&bounds));
        THROW_IF_FAILED(m_host->put_Bounds({ (LONG)p.x, (LONG)p.y, bounds.right, bounds.bottom }));
    }

    void webview_edge2::set_size(size s)
    {
        RECT bounds;
        THROW_IF_FAILED(m_host->get_Bounds(&bounds));
        THROW_IF_FAILED(m_host->put_Bounds({ bounds.left, bounds.top, bounds.left + (LONG)s.width, bounds.top + (LONG)s.height }));
    }

    void webview_edge2::set_rect(rectangle const& rect)
    {
        THROW_IF_FAILED(m_host->put_Bounds(to_native<RECT>(rect)));
    }

    void webview_edge2::go_forward()
    {
        THROW_IF_FAILED(m_view->GoForward());
    }

    void webview_edge2::go_back()
    {
        THROW_IF_FAILED(m_view->GoBack());
    }

    bool webview_edge2::get_can_go_forward()
    {
        BOOL value;
        THROW_IF_FAILED(m_view->get_CanGoForward(&value));
        return value;
    }

    bool webview_edge2::get_can_go_back()
    {
        BOOL value;
        THROW_IF_FAILED(m_view->get_CanGoBack(&value));
        return value;
    }
} // namespace xaml

#endif // XAML_UI_WEBVIEW_WEBVIEW2
