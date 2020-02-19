#include <wil/result_macros.h>
#include <win/webview_ie.hpp>

#include <ExDispID.h>

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
    {
        if (riid != IID_NULL)
            return DISP_E_UNKNOWNINTERFACE;
        switch (dispIdMember)
        {
        case DISPID_NAVIGATECOMPLETE2:
            m_webview->invoke_navigated(pDispParams->rgvarg->pvarVal->bstrVal);
            break;
        default:
            break;
        }
        return S_OK;
    }

    void webview_ie::create_async(HWND parent, rectangle const& rect, function<void()>&& callback)
    {
        try
        {
            RECT r = to_native<RECT>(rect);
            m_container.Create(parent, &r, 0, WS_CHILD | WS_VISIBLE);
            THROW_IF_FAILED(m_container.CreateControl(L"shell.Explorer.2"));
            THROW_IF_FAILED(m_container.QueryControl(&m_browser));
            wil::com_ptr<IConnectionPointContainer> spConnectionPointContainer;
            THROW_IF_FAILED(m_browser->QueryInterface(&spConnectionPointContainer));
            wil::com_ptr<IConnectionPoint> spConnectionPoint;
            THROW_IF_FAILED(spConnectionPointContainer->FindConnectionPoint(__uuidof(DWebBrowserEvents2), &spConnectionPoint));
            DWORD cookie;
            m_sink = make_unique<WebBrowserSink>(this);
            THROW_IF_FAILED(spConnectionPoint->Advise(m_sink.get(), &cookie));
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
