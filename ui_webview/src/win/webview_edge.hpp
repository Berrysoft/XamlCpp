#include <win/webview.hpp>

#include "winrt/Windows.Web.UI.Interop.h"

struct xaml_webview_edge : xaml_win32_webview
{
    winrt::Windows::Web::UI::Interop::WebViewControl m_view{ nullptr };

    xaml_result create_async(HWND parent, xaml_rectangle const& rect, std::function<xaml_result()>&& callback) noexcept override;

    operator bool() const noexcept override { return (bool)m_view; }

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
