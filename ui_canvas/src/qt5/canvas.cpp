#include <QPainterPath>
#include <QWidget>
#include <cmath>
#include <qt5/qstring.hpp>
#include <shared/canvas.hpp>
#include <xaml/ui/qt5/controls/brush.hpp>
#include <xaml/ui/qt5/controls/pen.hpp>

#if __has_include(<numbers>)
    #include <numbers>
using std::numbers::pi;
#else
    #define pi M_PI
#endif // __has_include(<numbers>)

using namespace std;

static xaml_result get_brush(QPainter*, xaml_brush* brush, xaml_rectangle const& region, QBrush* ptr) noexcept
{
    xaml_ptr<xaml_qt5_brush> native_brush;
    XAML_RETURN_IF_FAILED(brush->query(&native_brush));
    XAML_RETURN_IF_FAILED(native_brush->create(region, ptr));
    return XAML_S_OK;
}

static xaml_result set_pen(QPainter* handle, xaml_pen* pen, xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_qt5_pen> native_pen;
    XAML_RETURN_IF_FAILED(pen->query(&native_pen));
    QPen qpen;
    XAML_RETURN_IF_FAILED(native_pen->create(region, &qpen));
    handle->setPen(qpen);
    return XAML_S_OK;
}

#define CHECK_SIZE(r) \
    if ((r).width < 1 || (r).height < 1) return XAML_S_OK

static int get_drawing_angle(double angle) noexcept
{
    return (int)round(-angle * 180 / pi * 16);
}

xaml_result xaml_drawing_context_impl::draw_arc(xaml_pen* pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    XAML_RETURN_IF_FAILED(set_pen(m_handle, pen, region));
    m_handle->drawArc(xaml_to_native<QRectF>(region), get_drawing_angle(start_angle), get_drawing_angle(end_angle - start_angle));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_pie(xaml_brush* brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept
{
    CHECK_SIZE(region);
    QBrush qbrush;
    XAML_RETURN_IF_FAILED(get_brush(m_handle, brush, region, &qbrush));
    xaml_point center = { region.x + region.width / 2, region.y + region.height / 2 };
    QPainterPath path{ xaml_to_native<QPointF>(center) };
    path.arcTo(xaml_to_native<QRectF>(region), get_drawing_angle(start_angle), get_drawing_angle(end_angle - start_angle));
    path.closeSubpath();
    m_handle->fillPath(path, qbrush);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_ellipse(xaml_pen* pen, xaml_rectangle const& region) noexcept
{
    CHECK_SIZE(region);
    XAML_RETURN_IF_FAILED(set_pen(m_handle, pen, region));
    m_handle->drawEllipse(xaml_to_native<QRectF>(region));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_ellipse(xaml_brush* brush, xaml_rectangle const& region) noexcept
{
    CHECK_SIZE(region);
    QBrush qbrush;
    XAML_RETURN_IF_FAILED(get_brush(m_handle, brush, region, &qbrush));
    QPainterPath path;
    path.addEllipse(xaml_to_native<QRectF>(region));
    m_handle->fillPath(path, qbrush);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_line(xaml_pen* pen, xaml_point const& startp, xaml_point const& endp) noexcept
{
    xaml_point minp{ (min)(startp.x, endp.x), (min)(startp.y, endp.y) };
    xaml_point maxp{ (max)(startp.x, endp.x), (max)(startp.y, endp.y) };
    xaml_rectangle region = { minp.x, minp.y, maxp.x - minp.x, maxp.y - minp.y };
    XAML_RETURN_IF_FAILED(set_pen(m_handle, pen, region));
    m_handle->drawLine(xaml_to_native<QPointF>(startp), xaml_to_native<QPointF>(endp));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_rect(xaml_pen* pen, xaml_rectangle const& rect) noexcept
{
    CHECK_SIZE(rect);
    XAML_RETURN_IF_FAILED(set_pen(m_handle, pen, rect));
    m_handle->drawRect(xaml_to_native<QRectF>(rect));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_rect(xaml_brush* brush, xaml_rectangle const& rect) noexcept
{
    CHECK_SIZE(rect);
    QBrush qbrush;
    XAML_RETURN_IF_FAILED(get_brush(m_handle, brush, rect, &qbrush));
    m_handle->fillRect(xaml_to_native<QRectF>(rect), qbrush);
    return XAML_S_OK;
}

static QPainterPath get_round_rect_path(xaml_rectangle const& rect, xaml_size const& round)
{
    QPainterPath path{ QPointF{ rect.x + rect.width, rect.y + round.height } };
    path.arcTo(QRectF{ rect.x + rect.width - round.width, rect.y, round.width, round.height }, 0, 90);
    path.arcTo(QRectF{ rect.x, rect.y, round.width, round.height }, 90, 90);
    path.arcTo(QRectF{ rect.x, rect.y + rect.height - round.height, round.width, round.height }, 180, 90);
    path.arcTo(QRectF{ rect.x + rect.width - round.width, rect.y + rect.height - round.height, round.width, round.height }, 270, 90);
    path.closeSubpath();
    return path;
}

xaml_result xaml_drawing_context_impl::draw_round_rect(xaml_pen* pen, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    XAML_RETURN_IF_FAILED(set_pen(m_handle, pen, rect));
    m_handle->drawPath(get_round_rect_path(rect, round));
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::fill_round_rect(xaml_brush* brush, xaml_rectangle const& rect, xaml_size const& round) noexcept
{
    CHECK_SIZE(rect);
    CHECK_SIZE(round);
    QBrush qbrush;
    XAML_RETURN_IF_FAILED(get_brush(m_handle, brush, rect, &qbrush));
    m_handle->fillPath(get_round_rect_path(rect, round), qbrush);
    return XAML_S_OK;
}

xaml_result xaml_drawing_context_impl::draw_string(xaml_brush* brush, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str) noexcept
{
    if (font.size <= 0) return XAML_S_OK;
    QFont qfont{ font.font_family, -1, font.bold ? QFont::Bold : QFont::Normal, font.italic };
    qfont.setPixelSize((int)font.size);
    m_handle->setFont(qfont);
    QString text;
    XAML_RETURN_IF_FAILED(to_QString(str, &text));
    QFontMetricsF fm{ qfont };
    xaml_rectangle rect = xaml_from_native(fm.boundingRect(text));
    switch (font.halign)
    {
    case xaml_halignment_center:
        rect.x = p.x - rect.width / 2;
        break;
    case xaml_halignment_right:
        rect.x = p.x - rect.width;
        break;
    default:
        rect.x = p.x;
        break;
    }
    switch (font.valign)
    {
    case xaml_valignment_center:
        rect.y = p.y - rect.height / 2;
        break;
    case xaml_valignment_bottom:
        rect.y = p.y - rect.height;
        break;
    default:
        rect.y = p.y;
        break;
    }
    QBrush qbrush;
    XAML_RETURN_IF_FAILED(get_brush(m_handle, brush, rect, &qbrush));
    m_handle->setPen(QPen{ qbrush, 1.0 });
    QTextOption option;
    option.setWrapMode(QTextOption::NoWrap);
    m_handle->drawText(xaml_to_native<QRectF>(rect), text, option);
    return XAML_S_OK;
}

class XamlCanvas : public QWidget
{
private:
    xaml_canvas_internal* m_internal{};

public:
    XamlCanvas(xaml_canvas_internal* internal, QWidget* parent = nullptr) noexcept : QWidget(parent), m_internal(internal) {}

private:
    void paintEvent(QPaintEvent* event) override { m_internal->on_paint_event(event); }
};

xaml_result XAML_CALL xaml_canvas_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle = create<XamlCanvas>(this);
        XAML_RETURN_IF_FAILED(draw_visible());
    }
    return set_rect(region);
}

xaml_result XAML_CALL xaml_canvas_internal::invalidate(xaml_rectangle const*) noexcept
{
    m_handle->repaint();
    return XAML_S_OK;
}

void xaml_canvas_internal::on_paint_event(QPaintEvent*) noexcept
{
    QPainter painter{ m_handle.get() };
    xaml_ptr<xaml_drawing_context> dc;
    XAML_ASSERT_SUCCEEDED(xaml_object_new<xaml_drawing_context_impl>(&dc, &painter));
    XAML_ASSERT_SUCCEEDED(on_redraw(m_outer_this, dc));
}
