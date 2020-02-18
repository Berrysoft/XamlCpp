#include <xaml/ui/webview/webview.hpp>

#include <winrt/Windows.Web.UI.Interop.h>

namespace xaml
{
    class webview_edge : public native_webview
    {
    private:
        winrt::Windows::Web::UI::Interop::WebViewControl m_view{ nullptr };

    public:
        ~webview_edge() override {}

        void create_async(HWND parent, rectangle const& rect, std::function<void()>&& callback) override;

        operator bool() const override { return (bool)m_view; }

        void navigate(string_view_t uri) override;

        void set_location(point p) override;
        void set_size(size s) override;
        void set_rect(rectangle const& rect) override;
    };
} // namespace xaml
