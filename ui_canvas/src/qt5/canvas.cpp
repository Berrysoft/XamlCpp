#include <QWidget>
#include <shared/canvas.hpp>

class XamlCanvas : public QWidget
{
private:
    xaml_canvas_internal* m_internal{};

public:
    XamlCanvas(xaml_canvas_internal* internal) noexcept : QWidget(), m_internal(internal) {}

private:
    void paintEvent(QPaintEvent* event) override { m_internal->on_paint_event(event); }
};

xaml_result XAML_CALL xaml_canvas_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        m_handle.reset(new XamlCanvas(this));
        XAML_RETURN_IF_FAILED(draw_visible());
    }
    return set_rect(region);
}

xaml_result XAML_CALL xaml_canvas_internal::invalidate(xaml_rectangle const* prect) noexcept
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
