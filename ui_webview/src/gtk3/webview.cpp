#include <shared/atomic_guard.hpp>
#include <webkit2/webkit2.h>
#include <xaml/ui/controls/webview.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void webview::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = webkit_web_view_new();
            set_handle(h);
            g_signal_connect(G_OBJECT(get_handle()->handle), "load-changed", G_CALLBACK(webview::on_load_changed), this);
            draw_visible();
            draw_uri();
        }
        rectangle real = region - get_margin();
        __set_size_noevent({ real.width, real.height });
        draw_size();
    }

    void webview::draw_uri()
    {
        webkit_web_view_load_uri(WEBKIT_WEB_VIEW(get_handle()->handle), get_uri().data());
    }

    void webview::on_load_changed(void* web_view, int load_event, void* data)
    {
        if (load_event == WEBKIT_LOAD_COMMITTED)
        {
            webview* self = (webview*)data;
            atomic_guard guard(self->m_navigating);
            if (!guard.test_and_set())
            {
                self->set_uri(webkit_web_view_get_uri(WEBKIT_WEB_VIEW(self->get_handle()->handle)));
            }
        }
    }

    void webview::go_forward()
    {
        webkit_web_view_go_forward(WEBKIT_WEB_VIEW(get_handle()->handle));
    }

    void webview::go_back()
    {
        webkit_web_view_go_back(WEBKIT_WEB_VIEW(get_handle()->handle));
    }

    bool webview::get_can_go_forward()
    {
        return webkit_web_view_can_go_forward(WEBKIT_WEB_VIEW(get_handle()->handle));
    }

    bool webview::get_can_go_back()
    {
        return webkit_web_view_can_go_back(WEBKIT_WEB_VIEW(get_handle()->handle));
    }
} // namespace xaml
