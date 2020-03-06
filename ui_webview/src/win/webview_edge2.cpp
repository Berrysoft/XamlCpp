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
    void webview_edge2::create_async(intptr_t parent, rectangle const& rect, function<void()>&& callback)
    {
        try
        {
            THROW_IF_FAILED(CreateCoreWebView2Environment(
                Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
                    [=](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                        try
                        {
                            THROW_IF_FAILED(result);
                            m_env = env;
                            THROW_IF_FAILED(env->CreateCoreWebView2Host(
                                (HWND)parent,
                                Callback<ICoreWebView2CreateCoreWebView2HostCompletedHandler>(
                                    [=](HRESULT result, ICoreWebView2Host* webview) -> HRESULT {
                                        try
                                        {
                                            THROW_IF_FAILED(result);
                                            m_view = webview;
                                            THROW_IF_FAILED(m_view->put_Bounds(to_native<RECT>(rect)));
                                            wil::com_ptr<ICoreWebView2> view;
                                            THROW_IF_FAILED(m_view->get_CoreWebView2(&view));
                                            EventRegistrationToken token;
                                            THROW_IF_FAILED(view->add_NavigationCompleted(
                                                Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                                    [this](ICoreWebView2*, ICoreWebView2NavigationCompletedEventArgs*) -> HRESULT {
                                                        try
                                                        {
                                                            wil::com_ptr<ICoreWebView2> view;
                                                            THROW_IF_FAILED(m_view->get_CoreWebView2(&view));
                                                            wil::unique_cotaskmem_string uri;
                                                            THROW_IF_FAILED(view->get_Source(&uri));
                                                            invoke_navigated(uri.get());
                                                            return S_OK;
                                                        }
                                                        CATCH_RETURN();
                                                    })
                                                    .Get(),
                                                &token));
                                            THROW_IF_FAILED(view->AddWebResourceRequestedFilter(L"*", CORE_WEBVIEW2_WEB_RESOURCE_CONTEXT_ALL));
                                            THROW_IF_FAILED(view->add_WebResourceRequested(
                                                Callback<ICoreWebView2WebResourceRequestedEventHandler>(
                                                    [this](ICoreWebView2*, ICoreWebView2WebResourceRequestedEventArgs* e) -> HRESULT {
                                                        try
                                                        {
                                                            resource_requested_args args{};
                                                            wil::com_ptr<ICoreWebView2WebResourceRequest> req;
                                                            THROW_IF_FAILED(e->get_Request(&req));

                                                            wil::unique_cotaskmem_string method;
                                                            THROW_IF_FAILED(req->get_Method(&method));
                                                            args.request.method = method.get();

                                                            wil::unique_cotaskmem_string uri;
                                                            THROW_IF_FAILED(req->get_Uri(&uri));
                                                            args.request.uri = uri.get();

                                                            wil::com_ptr<IStream> stream;
                                                            THROW_IF_FAILED(req->get_Content(&stream));
                                                            if (stream)
                                                            {
                                                                auto count = wil::stream_size(stream.get());
                                                                vector<std::byte> data(count);
                                                                THROW_IF_FAILED(stream->Read(data.data(), (ULONG)count, NULL));
                                                                args.request.data = data;
                                                            }

                                                            invoke_resource_requested(args);
                                                            if (args.response)
                                                            {
                                                                auto& res = *args.response;
                                                                wil::com_ptr<IStream> res_data = SHCreateMemStream((const BYTE*)res.data.data(), (UINT)res.data.size());
                                                                wil::com_ptr<ICoreWebView2WebResourceResponse> response;
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
        wil::com_ptr<ICoreWebView2> view;
        THROW_IF_FAILED(m_view->get_CoreWebView2(&view));
        THROW_IF_FAILED(view->Navigate(uri.data()));
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

#endif // XAML_UI_WEBVIEW_WEBVIEW2
