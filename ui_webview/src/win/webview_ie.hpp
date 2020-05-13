#include <atomic>
#include <wil/com.h>
#include <win/webview.hpp>

#include <ExDisp.h>
#include <atlbase.h>
#include <atlwin.h>

struct xaml_webview_ie;

class WebBrowserSink : public DWebBrowserEvents2
{
protected:
    std::atomic<ULONG> m_ref;
    xaml_webview_ie* m_webview;
    BOOL m_can_go_forward;
    BOOL m_can_go_back;

public:
    WebBrowserSink(xaml_webview_ie* view) : m_ref(0), m_webview(view) {}
    ~WebBrowserSink() {}

    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;

    ULONG STDMETHODCALLTYPE AddRef() override
    {
        return ++m_ref;
    }

    ULONG STDMETHODCALLTYPE Release() override
    {
        if (--m_ref)
        {
            return m_ref;
        }
        else
        {
            delete this;
            return 0;
        }
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

    STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) override;

    STDMETHODIMP get_CanGoForward(BOOL* pbValue)
    {
        *pbValue = m_can_go_forward;
        return S_OK;
    }

    STDMETHODIMP get_CanGoBack(BOOL* pbValue)
    {
        *pbValue = m_can_go_back;
        return S_OK;
    }
};

struct xaml_webview_ie : xaml_win32_webview
{
    CAxWindow m_container{};
    wil::com_ptr_t<IWebBrowser2, wil::err_returncode_policy> m_browser{ nullptr };
    wil::com_ptr_t<WebBrowserSink, wil::err_returncode_policy> m_sink{ nullptr };

    xaml_result create_async(HWND parent, xaml_rectangle const& rect, std::function<xaml_result()>&& callback) noexcept override;

    operator bool() const noexcept override { return (bool)m_browser; }

    xaml_result navigate(xaml_char_t const* uri) noexcept override;

    xaml_result set_visible(bool vis) noexcept override;

    xaml_result set_location(xaml_point const& p) noexcept override;
    xaml_result set_size(xaml_size const& s) noexcept override;
    xaml_result set_rect(xaml_rectangle const& rect) noexcept override;

    xaml_result go_forward() noexcept override;
    xaml_result go_back() noexcept override;

    xaml_result get_can_go_forward(bool*) noexcept override;
    xaml_result get_can_go_back(bool*) noexcept override;
};
