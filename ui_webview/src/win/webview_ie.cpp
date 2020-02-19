#include <wil/result_macros.h>
#include <win/webview_ie.hpp>

using namespace std;

CComModule _Module;

namespace xaml
{
    void webview_ie::create_async(HWND parent, rectangle const& rect, function<void()>&& callback)
    {
        RECT r = to_native<RECT>(rect);
        m_container.Create(parent, &r, 0, WS_CHILD | WS_VISIBLE);
        m_container.CreateControl(L"shell.Explorer.2");
        m_container.QueryControl(&m_browser);
        if (callback) callback();
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
