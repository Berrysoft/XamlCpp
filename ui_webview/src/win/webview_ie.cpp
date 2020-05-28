#include <wil/result_macros.h>
#include <win/webview_ie.hpp>
#include <xaml/result_win32.h>
#include <xaml/ui/drawing_conv.hpp>

#include <ExDispID.h>
#include <Shlwapi.h>

using namespace std;

CComModule _Module;

STDMETHODIMP WebBrowserSink::QueryInterface(REFIID riid, void** ppvObject)
{
    if (IsBadWritePtr(ppvObject, sizeof(void*)))
        return E_POINTER;
    *ppvObject = nullptr;
    if (riid == __uuidof(IUnknown) || riid == __uuidof(IDispatch) || riid == __uuidof(DWebBrowserEvents2))
    {
        AddRef();
        *ppvObject = this;
        return S_OK;
    }
    else
        return E_NOINTERFACE;
}

STDMETHODIMP WebBrowserSink::Invoke(DISPID dispIdMember, REFIID riid, LCID, WORD, DISPPARAMS* pDispParams, VARIANT*, EXCEPINFO*, UINT*)
{
    if (riid != IID_NULL)
        return DISP_E_UNKNOWNINTERFACE;
    switch (dispIdMember)
    {
    case DISPID_NAVIGATECOMPLETE2:
        return m_webview->invoke_navigated(pDispParams->rgvarg[0].pvarVal->bstrVal);
    case DISPID_BEFORENAVIGATE2:
    {
        xaml_ptr<xaml_webview_resource_requested_args> args;
        RETURN_IF_FAILED(xaml_webview_resource_requested_args_new(&args));
        xaml_ptr<xaml_webview_web_request> args_req;
        RETURN_IF_FAILED(xaml_webview_web_request_new(&args_req));
        RETURN_IF_FAILED(args->set_request(args_req));

        xaml_ptr<xaml_string> uri_str;
        RETURN_IF_FAILED(xaml_string_new(pDispParams->rgvarg[5].pvarVal->bstrVal, &uri_str));
        RETURN_IF_FAILED(args_req->set_uri(uri_str));
        xaml_ptr<xaml_string> method_str;
        RETURN_IF_FAILED(xaml_string_new(U("GET"), &method_str));
        RETURN_IF_FAILED(args_req->set_method(method_str));
        RETURN_IF_FAILED(m_webview->invoke_resource_requested(args));

        xaml_ptr<xaml_webview_web_response> args_res;
        RETURN_IF_FAILED(args->get_response(&args_res));
        if (args_res)
        {
            pDispParams->rgvarg[0].pvarVal->boolVal = VARIANT_TRUE;
            wil::com_ptr_t<IDispatch, wil::err_returncode_policy> pDisp = pDispParams->rgvarg[6].pdispVal;
            wil::com_ptr_t<IWebBrowser2, wil::err_returncode_policy> browser;
            RETURN_IF_FAILED(pDisp.query_to(&browser));
            RETURN_IF_FAILED(browser->Stop());
            RETURN_IF_FAILED(m_webview->navigate(U("about:blank")));
            wil::com_ptr_t<IDispatch, wil::err_returncode_policy> doc;
            RETURN_IF_FAILED(browser->get_Document(&doc));
            wil::com_ptr_t<IPersistStreamInit, wil::err_returncode_policy> psi;
            RETURN_IF_FAILED(doc.query_to(&psi));
            xaml_ptr<xaml_buffer> buffer;
            RETURN_IF_FAILED(args_res->get_data(&buffer));
            uint8_t* data;
            RETURN_IF_FAILED(buffer->get_data(&data));
            int32_t size;
            RETURN_IF_FAILED(buffer->get_size(&size));
            wil::com_ptr_t<IStream, wil::err_returncode_policy> res_data = SHCreateMemStream((const BYTE*)data, (UINT)size);
            RETURN_IF_FAILED(psi->Load(res_data.get()));
        }
        break;
    }
    case DISPID_COMMANDSTATECHANGE:
        switch (pDispParams->rgvarg[1].lVal)
        {
        case CSC_NAVIGATEFORWARD:
            m_can_go_forward = pDispParams->rgvarg[0].boolVal;
            break;
        case CSC_NAVIGATEBACK:
            m_can_go_back = pDispParams->rgvarg[0].boolVal;
            break;
        }
        break;
    default:
        break;
    }
    return S_OK;
}

xaml_result xaml_webview_ie::create_async(HWND parent, xaml_rectangle const& rect, function<xaml_result()>&& callback) noexcept
{
    RECT r = xaml_to_native<RECT>(rect);
    m_container.Create(parent, &r, 0, WS_CHILD | WS_VISIBLE);
    m_sink = new WebBrowserSink(this);
    wil::com_ptr_t<IUnknown, wil::err_returncode_policy> control;
    XAML_RETURN_IF_FAILED(m_container.CreateControlEx(L"Shell.Explorer.2", nullptr, nullptr, &control, __uuidof(DWebBrowserEvents2), m_sink.get()));
    XAML_RETURN_IF_FAILED(control.query_to<IWebBrowser2>(&m_browser));
    return callback();
}

xaml_result xaml_webview_ie::navigate(char const* uri) noexcept
{
    wil::unique_variant empty{};
    wstring data;
    XAML_RETURN_IF_FAILED(to_wstring(uri, &data));
    auto u = wil::make_bstr_nothrow(data.c_str());
    return m_browser->Navigate(u.get(), &empty, &empty, &empty, &empty);
}

xaml_result xaml_webview_ie::set_visible(bool vis) noexcept
{
    m_container.ShowWindow(vis ? SW_SHOWNORMAL : SW_HIDE);
    return XAML_S_OK;
}

xaml_result xaml_webview_ie::set_location(xaml_point const& p) noexcept
{
    XAML_RETURN_IF_WIN32_BOOL_FALSE(m_container.SetWindowPos(HWND_TOP, (int)p.x, (int)p.y, 0, 0, SWP_NOZORDER | SWP_NOSIZE));
    return XAML_S_OK;
}

xaml_result xaml_webview_ie::set_size(xaml_size const& s) noexcept
{
    XAML_RETURN_IF_WIN32_BOOL_FALSE(m_container.SetWindowPos(HWND_TOP, 0, 0, (int)s.width, (int)s.height, SWP_NOZORDER | SWP_NOMOVE));
    return XAML_S_OK;
}

xaml_result xaml_webview_ie::set_rect(xaml_rectangle const& rect) noexcept
{
    RECT r = xaml_to_native<RECT>(rect);
    XAML_RETURN_IF_WIN32_BOOL_FALSE(m_container.SetWindowPos(HWND_TOP, &r, SWP_NOZORDER));
    return XAML_S_OK;
}

xaml_result xaml_webview_ie::go_forward() noexcept
{
    return m_browser->GoForward();
}

xaml_result xaml_webview_ie::go_back() noexcept
{
    return m_browser->GoBack();
}

xaml_result xaml_webview_ie::get_can_go_forward(bool* pvalue) noexcept
{
    BOOL value;
    XAML_RETURN_IF_FAILED(m_sink->get_CanGoForward(&value));
    *pvalue = value;
    return XAML_S_OK;
}

xaml_result xaml_webview_ie::get_can_go_back(bool* pvalue) noexcept
{
    BOOL value;
    XAML_RETURN_IF_FAILED(m_sink->get_CanGoBack(&value));
    *pvalue = value;
    return XAML_S_OK;
}
