#include <wil/com.h>
#include <xaml/ui/controls/native_webview.hpp>

#include <WebView2.h>

namespace xaml
{
    class webview_edge2 : public native_webview
    {
    private:
        wil::com_ptr<ICoreWebView2Environment> m_env{ nullptr };
        wil::com_ptr<ICoreWebView2Host> m_host{ nullptr };
        wil::com_ptr<ICoreWebView2> m_view{ nullptr };

    public:
        ~webview_edge2() override {}

        void create_async(HWND parent, rectangle const& rect, std::function<void()>&& callback) override;

        operator bool() const override { return (bool)m_view; }

        void navigate(string_view_t uri) override;

        void set_location(point p) override;
        void set_size(size s) override;
        void set_rect(rectangle const& rect) override;
    };
} // namespace xaml
