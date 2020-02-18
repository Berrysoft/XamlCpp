#include <wil/com.h>
#include <xaml/ui/webview/webview.hpp>

#include <atlbase.h>
#include <atlwin.h>

namespace xaml
{
    class webview_ie : public native_webview
    {
    private:
        CAxWindow m_container{};
        wil::com_ptr<IWebBrowser2> m_browser{ nullptr };

    public:
        ~webview_ie() override {}

        void create_async(HWND parent, rectangle const& rect, std::function<void()>&& callback) override;

        operator bool() const override { return (bool)m_browser; }

        void navigate(string_view_t uri) override;

        void set_location(point p) override;
        void set_size(size s) override;
        void set_rect(rectangle const& rect) override
        {
            set_location({ rect.x, rect.y });
            set_size({ rect.width, rect.height });
        }
    };
} // namespace xaml
