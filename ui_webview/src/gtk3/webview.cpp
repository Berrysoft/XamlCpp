#include <shared/atomic_guard.hpp>
#include <webkit2/webkit2.h>
#include <xaml/ui/webview/webview.hpp>

using namespace std;

namespace xaml
{
    void webview::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            set_handle(webkit_web_view_new());
            draw_uri();
        }
        rectangle real = region - get_margin();
        __set_size_noevent({ real.width, real.height });
        draw_size();
    }

    void webview::draw_uri()
    {
        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(get_handle()), get_uri().data());
    }

    void webview::draw_size()
    {
        auto [rw, rh] = to_native<tuple<gint, gint>>(get_size());
        gtk_widget_set_size_request(get_handle(), rw, rh);
    }

    void webview::on_load_changed(WebKitWebView* web_view, WebKitLoadEvent load_event, gpointer data)
    {
        if (load_event == WEBKIT_LOAD_FINISHED)
        {
            webview* self = (webview*)data;
            atomic_guard guard(self->m_navigating);
            if (!guard.exchange(true))
            {
                self->set_uri(webkit_web_view_get_uri(web_view));
            }
        }
    }
} // namespace xaml
