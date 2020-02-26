#include <wil/result_macros.h>
#include <win/webview_ie.hpp>

#include <ExDispID.h>
#include <Shlwapi.h>

using namespace std;

CComModule _Module;

namespace xaml
{
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

    STDMETHODIMP WebBrowserSink::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr)
    try
    {
        if (riid != IID_NULL)
            return DISP_E_UNKNOWNINTERFACE;
        switch (dispIdMember)
        {
        case DISPID_NAVIGATECOMPLETE2:
            m_webview->invoke_navigated(pDispParams->rgvarg[0].pvarVal->bstrVal);
            break;
        case DISPID_BEFORENAVIGATE2:
        {
            resource_requested_args args{};
            args.request.uri = pDispParams->rgvarg[5].pvarVal->bstrVal;
            args.request.method = U("GET");
            m_webview->invoke_resource_requested(args);
            if (args.response)
            {
                auto& res = *args.response;
                pDispParams->rgvarg[0].pvarVal->boolVal = VARIANT_TRUE;
                IDispatch* pDisp = pDispParams->rgvarg[6].pdispVal;
                wil::com_ptr<IWebBrowser2> browser;
                THROW_IF_FAILED(pDisp->QueryInterface(&browser));
                THROW_IF_FAILED(browser->Stop());
                m_webview->navigate(U("about:blank"));
                wil::com_ptr<IDispatch> doc;
                THROW_IF_FAILED(browser->get_Document(&doc));
                wil::com_ptr<IPersistStreamInit> psi;
                THROW_IF_FAILED(doc->QueryInterface(&psi));
                wil::com_ptr<IStream> res_data = SHCreateMemStream((const BYTE*)res.data.data(), (UINT)res.data.size());
                THROW_IF_FAILED(psi->Load(res_data.get()));
            }
            break;
        }
        default:
            break;
        }
        return S_OK;
    }
    CATCH_RETURN();

    void webview_ie::create_async(HWND parent, rectangle const& rect, function<void()>&& callback)
    {
        try
        {
            RECT r = to_native<RECT>(rect);
            m_container.Create(parent, &r, 0, WS_CHILD | WS_VISIBLE);
            m_sink = new WebBrowserSink(this);
            wil::com_ptr<IUnknown> control;
            THROW_IF_FAILED(m_container.CreateControlEx(L"Shell.Explorer.2", nullptr, nullptr, &control, __uuidof(DWebBrowserEvents2), m_sink.get()));
            THROW_IF_FAILED(control->QueryInterface(&m_browser));
        }
        catch (wil::ResultException const&)
        {
            m_browser = nullptr;
        }
        callback();
    }

    void webview_ie::navigate(string_view_t uri)
    {
        wil::unique_variant empty{};
        auto u = wil::make_bstr(uri.data());
        THROW_IF_FAILED(m_browser->Navigate(u.get(), &empty, &empty, &empty, &empty));
    }

    void webview_ie::set_location(point p)
    {
        THROW_IF_FAILED(m_browser->put_Left((long)p.x));
        THROW_IF_FAILED(m_browser->put_Top((long)p.y));
    }

    void webview_ie::set_size(size s)
    {
        THROW_IF_FAILED(m_browser->put_Width((long)s.width));
        THROW_IF_FAILED(m_browser->put_Height((long)s.height));
    }
} // namespace xaml
