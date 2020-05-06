#include <win/webview.hpp>

#include "winrt/Windows.Web.UI.Interop.h"

struct xaml_webview_edge : xaml_win32_webview
{
    winrt::Windows::Web::UI::Interop::WebViewControl m_view{ nullptr };

    xaml_result create_async(HWND parent, xaml_rectangle const& rect, std::function<xaml_result()>&& callback) override;

    operator bool() const noexcept override { return (bool)m_view; }

    xaml_result navigate(xaml_char_t const* uri) override;

    xaml_result set_visible(bool vis) override;

    xaml_result set_location(xaml_point const& p) override;
    xaml_result set_size(xaml_size const& s) override;
    xaml_result set_rect(xaml_rectangle const& rect) override;

    xaml_result go_forward() override;
    xaml_result go_back() override;

    xaml_result get_can_go_forward(bool*) override;
    xaml_result get_can_go_back(bool*) override;
};
