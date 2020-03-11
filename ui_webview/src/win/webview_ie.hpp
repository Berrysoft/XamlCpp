#include <atomic>
#include <wil/com.h>
#include <xaml/ui/controls/native_webview.hpp>

#include <ExDisp.h>
#include <atlbase.h>
#include <atlwin.h>

namespace xaml
{
    class webview_ie;

    class WebBrowserSink : public DWebBrowserEvents2
    {
    protected:
        std::atomic<ULONG> m_ref;
        webview_ie* m_webview;

    public:
        WebBrowserSink(xaml::webview_ie* view) : m_ref(0), m_webview(view) {}
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
    };

    class webview_ie : public native_webview
    {
    private:
        CAxWindow m_container{};
        wil::com_ptr<IWebBrowser2> m_browser{ nullptr };
        wil::com_ptr<WebBrowserSink> m_sink{ nullptr };

    public:
        ~webview_ie() override {}

        void create_async(HWND parent, rectangle const& rect, std::function<void()>&& callback) override;

        operator bool() const override { return (bool)m_browser; }

        void navigate(string_view_t uri) override;

        void set_visible(bool vis) override;

        void set_location(point p) override;
        void set_size(size s) override;
        void set_rect(rectangle const& rect) override;
    };
} // namespace xaml
