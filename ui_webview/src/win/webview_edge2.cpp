#ifdef XAML_UI_WEBVIEW_WEBVIEW2

#include <Shlwapi.h>
#include <wil/result_macros.h>
#include <win/webview_edge2.hpp>
#include <wrl/event.h>
#include <xaml/ui/native_drawing.hpp>

using namespace std;
using namespace Microsoft::WRL;

xaml_result xaml_webview_edge2::create_async(HWND parent, xaml_rectangle const& rect, function<xaml_result()>&& callback) noexcept
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
                            RETURN_IF_FAILED(m_host->put_Bounds(xaml_to_native<RECT>(rect)));
                            RETURN_IF_FAILED(m_host->get_CoreWebView2(&m_view));
                            EventRegistrationToken token;
                            RETURN_IF_FAILED(m_view->add_NavigationCompleted(
                                Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                    [this](ICoreWebView2*, ICoreWebView2NavigationCompletedEventArgs*) -> HRESULT {
                                        wil::unique_cotaskmem_string uri;
                                        RETURN_IF_FAILED(m_view->get_Source(&uri));
                                        return invoke_navigated(uri.get());
                                    })
                                    .Get(),
                                &token));
                            RETURN_IF_FAILED(m_view->AddWebResourceRequestedFilter(L"*", CORE_WEBVIEW2_WEB_RESOURCE_CONTEXT_ALL));
                            RETURN_IF_FAILED(m_view->add_WebResourceRequested(
                                Callback<ICoreWebView2WebResourceRequestedEventHandler>(
                                    [this](ICoreWebView2*, ICoreWebView2WebResourceRequestedEventArgs* e) -> HRESULT {
                                        xaml_ptr<xaml_webview_resource_requested_args> args;
                                        RETURN_IF_FAILED(xaml_webview_resource_requested_args_new(&args));

                                        xaml_ptr<xaml_webview_web_request> args_req;
                                        RETURN_IF_FAILED(xaml_webview_web_request_new(&args_req));
                                        RETURN_IF_FAILED(args->set_request(args_req.get()));

                                        wil::com_ptr<ICoreWebView2WebResourceRequest> req;
                                        RETURN_IF_FAILED(e->get_Request(&req));

                                        wil::unique_cotaskmem_string method;
                                        RETURN_IF_FAILED(req->get_Method(&method));
                                        xaml_ptr<xaml_string> method_str;
                                        RETURN_IF_FAILED(xaml_string_new(method.get(), &method_str));
                                        RETURN_IF_FAILED(args_req->set_method(method_str.get()));

                                        wil::unique_cotaskmem_string uri;
                                        RETURN_IF_FAILED(req->get_Uri(&uri));
                                        xaml_ptr<xaml_string> uri_str;
                                        RETURN_IF_FAILED(xaml_string_new(uri.get(), &uri_str));
                                        RETURN_IF_FAILED(args_req->set_uri(uri_str.get()));

                                        wil::com_ptr<IStream> stream;
                                        RETURN_IF_FAILED(req->get_Content(&stream));
                                        if (stream)
                                        {
                                            unsigned long long count;
                                            RETURN_IF_FAILED(wil::stream_size_nothrow(stream.get(), &count));
                                            xaml_ptr<xaml_buffer> buffer;
                                            RETURN_IF_FAILED(xaml_buffer_new((int32_t)count, &buffer));
                                            uint8_t* data;
                                            RETURN_IF_FAILED(buffer->get_data(&data));
                                            RETURN_IF_FAILED(stream->Read(data, (ULONG)count, nullptr));
                                            RETURN_IF_FAILED(args_req->set_data(buffer.get()));
                                        }

                                        RETURN_IF_FAILED(invoke_resource_requested(args));

                                        xaml_ptr<xaml_webview_web_response> args_res;
                                        RETURN_IF_FAILED(args->get_response(&args_res));
                                        if (args_res)
                                        {
                                            xaml_ptr<xaml_buffer> buffer;
                                            RETURN_IF_FAILED(args_res->get_data(&buffer));
                                            uint8_t* data;
                                            RETURN_IF_FAILED(buffer->get_data(&data));
                                            int32_t size;
                                            RETURN_IF_FAILED(buffer->get_size(&size));
                                            wil::com_ptr<IStream> res_data = SHCreateMemStream((const BYTE*)data, (UINT)size);
                                            xaml_ptr<xaml_string> ct_str;
                                            RETURN_IF_FAILED(args_res->get_content_type(&ct_str));
                                            xaml_char_t const* ct;
                                            RETURN_IF_FAILED(ct_str->get_data(&ct));
                                            wil::com_ptr<ICoreWebView2WebResourceResponse> response;
                                            RETURN_IF_FAILED(m_env->CreateWebResourceResponse(res_data.get(), 200, L"OK", (L"Content-Type: " + (xaml_std_string_t)ct).c_str(), &response));
                                            RETURN_IF_FAILED(e->put_Response(response.get()));
                                        }
                                        return S_OK;
                                    })
                                    .Get(),
                                &token));
                            return callback();
                        })
                        .Get()));
                return S_OK;
            })
            .Get());
    if (FAILED(hr))
    {
        m_view = nullptr;
        XAML_RETURN_IF_FAILED(callback());
    }
    return hr;
}

xaml_result xaml_webview_edge2::navigate(xaml_char_t const* uri) noexcept
{
    return m_view->Navigate(uri);
}

xaml_result xaml_webview_edge2::set_visible(bool vis) noexcept
{
    return m_host->put_IsVisible(vis);
}

xaml_result xaml_webview_edge2::set_location(xaml_point const& p) noexcept
{
    RECT bounds;
    XAML_RETURN_IF_FAILED(m_host->get_Bounds(&bounds));
    XAML_RETURN_IF_FAILED(m_host->put_Bounds({ (LONG)p.x, (LONG)p.y, bounds.right, bounds.bottom }));
    return XAML_S_OK;
}

xaml_result xaml_webview_edge2::set_size(xaml_size const& s) noexcept
{
    RECT bounds;
    XAML_RETURN_IF_FAILED(m_host->get_Bounds(&bounds));
    XAML_RETURN_IF_FAILED(m_host->put_Bounds({ bounds.left, bounds.top, bounds.left + (LONG)s.width, bounds.top + (LONG)s.height }));
    return XAML_S_OK;
}

xaml_result xaml_webview_edge2::set_rect(xaml_rectangle const& rect) noexcept
{
    return m_host->put_Bounds(xaml_to_native<RECT>(rect));
}

xaml_result xaml_webview_edge2::go_forward() noexcept
{
    return m_view->GoForward();
}

xaml_result xaml_webview_edge2::go_back() noexcept
{
    return m_view->GoBack();
}

xaml_result xaml_webview_edge2::get_can_go_forward(bool* pvalue) noexcept
{
    BOOL value;
    XAML_RETURN_IF_FAILED(m_view->get_CanGoForward(&value));
    *pvalue = value;
    return XAML_S_OK;
}

xaml_result xaml_webview_edge2::get_can_go_back(bool* pvalue) noexcept
{
    BOOL value;
    XAML_RETURN_IF_FAILED(m_view->get_CanGoBack(&value));
    *pvalue = value;
    return XAML_S_OK;
}

#endif // XAML_UI_WEBVIEW_WEBVIEW2
