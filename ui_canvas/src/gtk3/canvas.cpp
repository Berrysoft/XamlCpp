#include <cairo.h>
#include <cmath>
#include <numbers>
#include <shared/canvas.hpp>
#include <xaml/ui/controls/canvas.h>

using namespace std;
using std::numbers::pi;

static void set_pen(cairo_t* handle, xaml_drawing_pen const& pen) noexcept
{
    cairo_set_source_rgba(handle, pen.stroke.r / 255.0, pen.stroke.g / 255.0, pen.stroke.b / 255.0, pen.stroke.a / 255.0);
    cairo_set_line_width(handle, pen.width);
}

static void set_brush(cairo_t* handle, xaml_drawing_brush const& brush) noexcept
{
    cairo_set_source_rgba(handle, brush.fill.r / 255.0, brush.fill.g / 255.0, brush.fill.b / 255.0, brush.fill.a / 255.0);
}

static void path_arc(cairo_t* handle, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    cairo_matrix_t save_matrix;
    cairo_get_matrix(handle, &save_matrix);
    cairo_scale(handle, 1, region.height / region.width);
    cairo_new_path(handle);
    cairo_arc(handle, region.x + region.width / 2, region.y + region.height / 2, region.width / 2, start_angle, end_angle);
    cairo_set_matrix(handle, &save_matrix);
}

xaml_result xaml_drawing_context_impl::draw_arc(xaml_drawing_pen const& pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    path_arc(m_handle, region, start_angle, end_angle);
    set_pen(m_handle, pen);
    cairo_stroke(m_handle);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_pie(xaml_drawing_brush const& brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    path_arc(m_handle, region, start_angle, end_angle);
    set_brush(m_handle, brush);
    cairo_fill(m_handle);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_ellipse(xaml_drawing_pen const& pen, xaml_rectangle const& region) noexcept
{
    return draw_arc(pen, region, 0, 2 * pi);
}

xaml_result xaml_drawing_context_impl::fill_ellipse(xaml_drawing_brush const& brush, const xaml_rectangle& region) noexcept
{
    return fill_pie(brush, region, 0, 2 * pi);
}

xaml_result xaml_drawing_context_impl::draw_line(xaml_drawing_pen const& pen, xaml_point const& startp, xaml_point const& endp) noexcept
{
    cairo_new_path(m_handle);
    cairo_move_to(m_handle, startp.x, startp.y);
    cairo_line_to(m_handle, endp.x, endp.y);
    set_pen(m_handle, pen);
    cairo_stroke(m_handle);
    return XAML_S_OK;
}

static void path_rect(cairo_t* handle, xaml_rectangle const& rect) noexcept
{
    cairo_new_path(handle);
    cairo_rectangle(handle, rect.x, rect.y, rect.width, rect.height);
}

xaml_result xaml_drawing_context_impl::draw_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect) noexcept
{
    path_rect(m_handle, rect);
    set_pen(m_handle, pen);
    cairo_stroke(m_handle);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_rect(xaml_drawing_brush const& brush, const xaml_rectangle& rect) noexcept
{
    path_rect(m_handle, rect);
    set_brush(m_handle, brush);
    cairo_stroke(m_handle);
    return XAML_S_OK;
}

static void path_round_rect(cairo_t* handle, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    cairo_matrix_t save_matrix;
    cairo_get_matrix(handle, &save_matrix);
    cairo_scale(handle, 1, round.height / round.width);
    cairo_new_sub_path(handle);
    cairo_arc(handle, rect.x + rect.width - round.width / 2, rect.y + round.height / 2, round.width / 2, -pi / 2, 0);
    cairo_arc(handle, rect.x + rect.width - round.width / 2, rect.y + rect.height - round.height / 2, round.width / 2, 0, pi / 2);
    cairo_arc(handle, rect.x + round.width / 2, rect.y + rect.height - round.height / 2, round.width / 2, pi / 2, pi);
    cairo_arc(handle, rect.x + round.width / 2, rect.y + round.height / 2, round.width / 2, pi, pi / 2 * 3);
    cairo_close_path(handle);
    cairo_set_matrix(handle, &save_matrix);
}

xaml_result xaml_drawing_context_impl::draw_round_rect(xaml_drawing_pen const& pen, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    path_round_rect(m_handle, rect, round);
    set_pen(m_handle, pen);
    cairo_stroke(m_handle);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_round_rect(xaml_drawing_brush const& brush, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    path_round_rect(m_handle, rect, round);
    set_brush(m_handle, brush);
    cairo_stroke(m_handle);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_string(xaml_drawing_brush const& brush, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str) noexcept
{
    set_brush(m_handle, brush);
    cairo_select_font_face(m_handle, font.font_family, font.italic ? CAIRO_FONT_SLANT_ITALIC : CAIRO_FONT_SLANT_NORMAL, font.bold ? CAIRO_FONT_WEIGHT_BOLD : CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(m_handle, font.size);
    char const* data;
    XAML_RETURN_IF_FAILED(str->get_data(&data));
    cairo_text_extents_t extent;
    cairo_text_extents(m_handle, data, &extent);
    auto [x, y] = p;
    switch (font.halign)
    {
    case xaml_halignment_center:
        x -= extent.width / 2;
        break;
    case xaml_halignment_right:
        x -= extent.width;
        break;
    default:
        break;
    }
    switch (font.valign)
    {
    case xaml_valignment_center:
        y += extent.height / 2;
        break;
    case xaml_valignment_top:
        y += extent.height;
        break;
    default:
        break;
    }
    cairo_move_to(m_handle, x, y);
    cairo_show_text(m_handle, data);
    return XAML_S_OK;
}

xaml_result xaml_canvas_internal::draw(const xaml_rectangle& region) noexcept
{
    if (!m_handle)
    {
        m_handle = gtk_drawing_area_new();
        g_signal_connect(G_OBJECT(m_handle), "draw", G_CALLBACK(xaml_canvas_internal::on_draw), this);
        XAML_RETURN_IF_FAILED(draw_visible());
    }
    return set_rect(region);
}

gboolean xaml_canvas_internal::on_draw(GtkWidget* widget, cairo_t* cr, xaml_canvas_internal* self) noexcept
{
    xaml_ptr<xaml_drawing_context> dc;
    XAML_ASSERT_SUCCEEDED(xaml_object_new<xaml_drawing_context_impl>(&dc, cr));
    XAML_ASSERT_SUCCEEDED(self->on_redraw(self->m_outer_this, dc));
    return FALSE;
}
