#include <atomic>
#include <wil/result_macros.h>
#include <win/webview_ie.hpp>

#include <ExDispID.h>

using namespace std;

CComModule _Module;

class WebBrowserSink : public DWebBrowserEvents2
{
protected:
    atomic<ULONG> m_ref;
    xaml::webview_ie* m_webview;

public:
    WebBrowserSink(xaml::webview_ie* view) : m_ref(1), m_webview(view) {}
    ~WebBrowserSink() {}

    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override
    {
        if (IsBadWritePtr(ppvObject, sizeof(void*))) return E_POINTER;
        HRESULT res = S_OK;
        *ppvObject = nullptr;
        if (riid == __uuidof(IUnknown) || riid == __uuidof(IDispatch) || riid == __uuidof(DWebBrowserEvents2))
            *ppvObject = this;
        else
            res = E_NOINTERFACE;
        return res;
    }

    ULONG STDMETHODCALLTYPE AddRef() override
    {
        return ++m_ref;
    }

    ULONG STDMETHODCALLTYPE Release() override
    {
        if (!(--m_ref))
        {
            delete this;
        }
        return m_ref;
    }

    STDMETHODIMP GetTypeInfoCount(UINT* pctinfo) override
    {
        *pctinfo = 0;
        return S_OK;
    }

    STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) override
    {
        *ppTInfo = NULL;
        return E_NOTIMPL;
    }

    STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
    {
        return E_NOTIMPL;
    }

    STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) override
    {
        if (riid != IID_NULL)
            return DISP_E_UNKNOWNINTERFACE;
        switch (dispIdMember)
        {
        case DISPID_NAVIGATECOMPLETE2:
            m_webview->invoke_navigated((LPOLESTR)pDispParams->rgvarg[1].pbstrVal);
            break;
        default:
            break;
        }

        return S_OK;
    }
};

namespace xaml
{
    void webview_ie::create_async(HWND parent, rectangle const& rect, function<void()>&& callback)
    {
        try
        {
            RECT r = to_native<RECT>(rect);
            m_container.Create(parent, &r, 0, WS_CHILD | WS_VISIBLE);
            THROW_IF_FAILED(m_container.CreateControl(L"shell.Explorer.2"));
            THROW_IF_FAILED(m_container.QueryControl(&m_browser));
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
