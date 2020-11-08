#ifdef XAML_UI_WEBVIEW_EDGE

    #define _ROAPI_

    #include <robuffer.h>
    #include <win/webview_edge.hpp>

    #include <Windows.Foundation.h>
    #include <Windows.Storage.Streams.h>
    #include <Windows.Web.Http.h>
    #include <Windows.Web.UI.h>
    #include <wrl.h>
    #include <xaml/internal/string.hpp>
    #include <xaml/ui/win/winrt.h>

using namespace std;
using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Storage::Streams;
using namespace ABI::Windows::Web::Http;
using namespace ABI::Windows::Web::UI;
using namespace ABI::Windows::Web::UI::Interop;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

struct ArrayViewBuffer : RuntimeClass<RuntimeClassFlags<WinRtClassicComMix>, IBuffer, Windows::Storage::Streams::IBufferByteAccess>
{
    xaml_ptr<xaml_buffer> m_buffer;

    ArrayViewBuffer(xaml_ptr<xaml_buffer> const& buffer) : m_buffer(buffer) {}

    HRESULT STDMETHODCALLTYPE get_Length(UINT32* value) override
    {
        int32_t size;
        RETURN_IF_FAILED(m_buffer->get_size(&size));
        *value = (UINT32)size;
        return XAML_S_OK;
    }

    HRESULT STDMETHODCALLTYPE get_Capacity(UINT32* value) override
    {
        return get_Length(value);
    }

    HRESULT STDMETHODCALLTYPE put_Length(UINT32) override
    {
        return E_INVALIDARG;
    }

    STDMETHODIMP Buffer(::byte** value) override
    {
        return m_buffer->get_data(value);
    }
};

xaml_result xaml_webview_edge::create_async(HWND parent, xaml_rectangle const& rect, __xaml_function_wrapper_t<xaml_result() noexcept> callback) noexcept
{
    if (!XamlInitializeWinRTFunc()) return XAML_E_NOTIMPL;
    wil::com_ptr_nothrow<IWebViewControlProcess> process;
    XAML_RETURN_IF_FAILED(ActivateInstance(HStringReference(RuntimeClass_Windows_Web_UI_Interop_WebViewControlProcess).Get(), &process));
    wil::com_ptr_nothrow<IAsyncOperation<WebViewControl*>> task;
    XAML_RETURN_IF_FAILED(process->CreateWebViewControlAsync((INT64)parent, { (float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height }, &task));
    HRESULT __hr = task->put_Completed(
        Callback<IAsyncOperationCompletedHandler<WebViewControl*>>(
            [this, callback](IAsyncOperation<WebViewControl*>* operation, AsyncStatus status) mutable noexcept -> HRESULT {
                if (status == AsyncStatus::Completed)
                {
                    RETURN_IF_FAILED(operation->GetResults(m_view.put()));
                    EventRegistrationToken token;
                    RETURN_IF_FAILED(m_view->add_NavigationCompleted(
                        Callback<ITypedEventHandler<IWebViewControl*, WebViewControlNavigationCompletedEventArgs*>>(
                            [this](IWebViewControl*, IWebViewControlNavigationCompletedEventArgs* args) noexcept -> HRESULT {
                                wil::com_ptr_nothrow<IUriRuntimeClass> uri;
                                RETURN_IF_FAILED(args->get_Uri(&uri));
                                HString absuri;
                                RETURN_IF_FAILED(uri->get_AbsoluteUri(absuri.GetAddressOf()));
                                return invoke_navigated(absuri.GetRawBuffer(nullptr));
                            })
                            .Get(),
                        &token));
                    RETURN_IF_FAILED(m_view->add_WebResourceRequested(
                        Callback<ITypedEventHandler<IWebViewControl*, WebViewControlWebResourceRequestedEventArgs*>>(
                            [this](IWebViewControl*, IWebViewControlWebResourceRequestedEventArgs* e) noexcept -> HRESULT {
                                wil::com_ptr_nothrow<IDeferral> deferral;
                                RETURN_IF_FAILED(e->GetDeferral(&deferral));
                                wil::com_ptr_nothrow<IHttpRequestMessage> req;
                                RETURN_IF_FAILED(e->get_Request(&req));
                                HString method;
                                {
                                    wil::com_ptr_nothrow<IHttpMethod> m;
                                    RETURN_IF_FAILED(req->get_Method(&m));
                                    RETURN_IF_FAILED(m->get_Method(method.GetAddressOf()));
                                }
                                HString uri;
                                {
                                    wil::com_ptr_nothrow<IUriRuntimeClass> u;
                                    RETURN_IF_FAILED(req->get_RequestUri(&u));
                                    RETURN_IF_FAILED(u->get_AbsoluteUri(uri.GetAddressOf()));
                                }
                                xaml_ptr<xaml_webview_resource_requested_args> args;
                                XAML_RETURN_IF_FAILED(xaml_webview_resource_requested_args_new(&args));
                                xaml_ptr<xaml_webview_web_request> args_req;
                                XAML_RETURN_IF_FAILED(xaml_webview_web_request_new(&args_req));
                                XAML_RETURN_IF_FAILED(args->set_request(args_req));

                                xaml_ptr<xaml_string> method_str;
                                XAML_RETURN_IF_FAILED(xaml_string_new(method.GetRawBuffer(nullptr), &method_str));
                                XAML_RETURN_IF_FAILED(args_req->set_method(method_str));
                                xaml_ptr<xaml_string> uri_str;
                                XAML_RETURN_IF_FAILED(xaml_string_new(uri.GetRawBuffer(nullptr), &uri_str));
                                XAML_RETURN_IF_FAILED(args_req->set_uri(uri_str));

                                XAML_RETURN_IF_FAILED(invoke_resource_requested(args));

                                xaml_ptr<xaml_webview_web_response> args_res;
                                XAML_RETURN_IF_FAILED(args->get_response(&args_res));
                                if (args_res)
                                {
                                    xaml_ptr<xaml_buffer> b;
                                    XAML_RETURN_IF_FAILED(args_res->get_data(&b));
                                    wil::com_ptr_nothrow<ArrayViewBuffer> buffer = Make<ArrayViewBuffer>(b);
                                    if (!buffer) return E_OUTOFMEMORY;
                                    wil::com_ptr_nothrow<IHttpResponseMessage> message;
                                    RETURN_IF_FAILED(ActivateInstance(HStringReference(RuntimeClass_Windows_Web_Http_HttpResponseMessage).Get(), &message));
                                    wil::com_ptr_nothrow<IHttpContent> content;
                                    {
                                        wil::com_ptr_nothrow<IHttpBufferContentFactory> content_factory;
                                        RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_Web_Http_HttpBufferContent).Get(), &content_factory));
                                        RETURN_IF_FAILED(content_factory->CreateFromBuffer(buffer.get(), &content));
                                    }
                                    RETURN_IF_FAILED(message->put_Content(content.get()));
                                    RETURN_IF_FAILED(e->put_Response(message.get()));
                                }

                                return deferral->Complete();
                            })
                            .Get(),
                        &token));
                }
                return callback();
            })
            .Get());
    if (FAILED(__hr))
    {
        m_view = nullptr;
        XAML_RETURN_IF_FAILED(callback());
    }
    return __hr;
}

xaml_result xaml_webview_edge::navigate(char const* uri) noexcept
{
    nowide::wstackstring data;
    XAML_RETURN_IF_FAILED(to_wstring(uri, &data));
    wil::com_ptr_nothrow<IUriRuntimeClassFactory> uri_factory;
    XAML_RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_Foundation_Uri).Get(), &uri_factory));
    wil::com_ptr_nothrow<IUriRuntimeClass> uri_class;
    XAML_RETURN_IF_FAILED(uri_factory->CreateUri(HStringReference(data.c_str()).Get(), &uri_class));
    return m_view->Navigate(uri_class.get());
}

xaml_result xaml_webview_edge::set_visible(bool vis) noexcept
{
    wil::com_ptr_nothrow<IWebViewControlSite> view;
    XAML_RETURN_IF_FAILED(m_view.query_to(&view));
    return view->put_IsVisible(vis);
}

xaml_result xaml_webview_edge::set_location(xaml_point const& p) noexcept
{
    wil::com_ptr_nothrow<IWebViewControlSite> view;
    XAML_RETURN_IF_FAILED(m_view.query_to(&view));
    Rect bounds;
    XAML_RETURN_IF_FAILED(view->get_Bounds(&bounds));
    return view->put_Bounds({ (FLOAT)p.x, (FLOAT)p.y, bounds.Width, bounds.Height });
}

xaml_result xaml_webview_edge::set_size(xaml_size const& s) noexcept
{
    wil::com_ptr_nothrow<IWebViewControlSite> view;
    XAML_RETURN_IF_FAILED(m_view.query_to(&view));
    Rect bounds;
    XAML_RETURN_IF_FAILED(view->get_Bounds(&bounds));
    return view->put_Bounds({ bounds.X, bounds.Y, (FLOAT)s.width, (FLOAT)s.height });
}

xaml_result xaml_webview_edge::set_rect(xaml_rectangle const& rect) noexcept
{
    wil::com_ptr_nothrow<IWebViewControlSite> view;
    XAML_RETURN_IF_FAILED(m_view.query_to(&view));
    return view->put_Bounds({ (float)rect.x, (float)rect.y, (float)rect.width, (float)rect.height });
}

xaml_result xaml_webview_edge::go_forward() noexcept
{
    return m_view->GoForward();
}

xaml_result xaml_webview_edge::go_back() noexcept
{
    return m_view->GoBack();
}

xaml_result xaml_webview_edge::get_can_go_forward(bool* pvalue) noexcept
{
    boolean value;
    XAML_RETURN_IF_FAILED(m_view->get_CanGoForward(&value));
    *pvalue = value;
    return XAML_S_OK;
}

xaml_result xaml_webview_edge::get_can_go_back(bool* pvalue) noexcept
{
    boolean value;
    XAML_RETURN_IF_FAILED(m_view->get_CanGoBack(&value));
    *pvalue = value;
    return XAML_S_OK;
}

#endif // XAML_UI_WEBVIEW_EDGE
