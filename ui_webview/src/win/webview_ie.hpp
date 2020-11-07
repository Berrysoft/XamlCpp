#include <atomic>
#include <wil/com.h>
#include <win/webview.hpp>

#include <ExDisp.h>
#include <atlbase.h>
#include <atlwin.h>

#include <wrl.h>

struct xaml_webview_ie;

struct WebBrowserSink : Microsoft::WRL::RuntimeClass<Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>, DWebBrowserEvents2>
{
    xaml_webview_ie* m_webview;
    BOOL m_can_go_forward;
    BOOL m_can_go_back;

    WebBrowserSink(xaml_webview_ie* view) : m_webview(view) {}

    STDMETHODIMP GetTypeInfoCount(UINT* pctinfo) override
    {
        *pctinfo = 0;
        return S_OK;
    }

    STDMETHODIMP GetTypeInfo(UINT, LCID, ITypeInfo** ppTInfo) override
    {
        *ppTInfo = NULL;
        return E_NOTIMPL;
    }

    STDMETHODIMP GetIDsOfNames(REFIID, LPOLESTR*, UINT, LCID, DISPID*)
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
    wil::com_ptr_nothrow<IWebBrowser2> m_browser{ nullptr };
    wil::com_ptr_nothrow<WebBrowserSink> m_sink{ nullptr };

    xaml_result create_async(HWND parent, xaml_rectangle const& rect, fu2::function<xaml_result() noexcept> callback) noexcept override;

    operator bool() const noexcept override { return (bool)m_browser; }

    xaml_result navigate(char const* uri) noexcept override;

    xaml_result set_visible(bool vis) noexcept override;

    xaml_result set_location(xaml_point const& p) noexcept override;
    xaml_result set_size(xaml_size const& s) noexcept override;
    xaml_result set_rect(xaml_rectangle const& rect) noexcept override;

    xaml_result go_forward() noexcept override;
    xaml_result go_back() noexcept override;

    xaml_result get_can_go_forward(bool*) noexcept override;
    xaml_result get_can_go_back(bool*) noexcept override;
};
