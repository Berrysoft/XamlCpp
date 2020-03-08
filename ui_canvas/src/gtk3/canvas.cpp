#include <gtk3/canvas_cairo.hpp>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/controls/native_canvas.hpp>
#include <xaml/ui/native_control.hpp>

using namespace std;

namespace xaml
{
    void canvas::__draw(const rectangle& region)
    {
        if (!get_handle())
        {
            auto h = make_shared<native_control>();
            h->handle = gtk_drawing_area_new();
            set_handle(h);
            g_signal_connect(G_OBJECT(get_handle()->handle), "draw", G_CALLBACK(canvas::on_draw), this);
        }
        m_real_region = region - get_margin();
        __set_rect(region);
    }

    int canvas::on_draw(void* widget, void* cr, void* data)
    {
        canvas* self = (canvas*)data;
        drawing_context_cairo ctx;
        ctx.handle = (cairo_t*)cr;
        drawing_context cv{ &ctx };
        self->m_redraw(*self, cv);
        return FALSE;
    }
} // namespace xaml
