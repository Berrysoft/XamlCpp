#include <WebView2.h>
#include <wil/com.h>
#include <win/webview.hpp>

struct xaml_webview_edge2 : xaml_win32_webview
{
    wil::com_ptr_t<ICoreWebView2Environment, wil::err_returncode_policy> m_env{ nullptr };
    wil::com_ptr_t<ICoreWebView2Host, wil::err_returncode_policy> m_host{ nullptr };
    wil::com_ptr_t<ICoreWebView2, wil::err_returncode_policy> m_view{ nullptr };

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
