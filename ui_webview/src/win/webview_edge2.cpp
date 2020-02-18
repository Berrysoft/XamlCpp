#include <wil/result_macros.h>
#include <win/webview_edge2.hpp>
#include <wrl/event.h>

using namespace std;
using namespace Microsoft::WRL;

namespace xaml
{
    void webview_edge2::create_async(HWND parent, rectangle const& rect, function<void()>&& callback)
    {
        THROW_IF_FAILED(CreateWebView2EnvironmentWithDetails(
            nullptr, nullptr, nullptr,
            Callback<IWebView2CreateWebView2EnvironmentCompletedHandler>(
                [=](HRESULT result, IWebView2Environment* env) -> HRESULT {
                    try
                    {
                        THROW_IF_FAILED(result);
                        m_env = env;
                        THROW_IF_FAILED(env->CreateWebView(
                            parent,
                            Callback<IWebView2CreateWebViewCompletedHandler>(
                                [=](HRESULT result, IWebView2WebView* webview) -> HRESULT {
                                    try
                                    {
                                        THROW_IF_FAILED(result);
                                        m_view = webview;
                                        THROW_IF_FAILED(m_view->put_Bounds(to_native<RECT>(rect)));
                                        if (callback) callback();
                                        return S_OK;
                                    }
                                    CATCH_RETURN();
                                })
                                .Get()));
                        return S_OK;
                    }
                    CATCH_RETURN();
                })
                .Get()));
    }

    void webview_edge2::navigate(string_view_t uri)
    {
        THROW_IF_FAILED(m_view->Navigate(uri.data()));
    }

    void webview_edge2::set_location(point p)
    {
        RECT bounds;
        THROW_IF_FAILED(m_view->get_Bounds(&bounds));
        THROW_IF_FAILED(m_view->put_Bounds({ (LONG)p.x, (LONG)p.y, bounds.right, bounds.bottom }));
    }

    void webview_edge2::set_size(size s)
    {
        RECT bounds;
        THROW_IF_FAILED(m_view->get_Bounds(&bounds));
        THROW_IF_FAILED(m_view->put_Bounds({ bounds.left, bounds.top, bounds.left + (LONG)s.width, bounds.top + (LONG)s.height }));
    }

    void webview_edge2::set_rect(rectangle const& rect)
    {
        THROW_IF_FAILED(m_view->put_Bounds(to_native<RECT>(rect)));
    }
} // namespace xaml
