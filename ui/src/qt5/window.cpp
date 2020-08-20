#include <QCloseEvent>
#include <QMainWindow>
#include <QMoveEvent>
#include <QResizeEvent>
#include <QScreen>
#include <qt5/qstring.hpp>
#include <shared/atomic_guard.hpp>
#include <shared/window.hpp>

class XamlMainWindow : public QMainWindow
{
private:
    xaml_window_internal* m_internal{};

public:
    XamlMainWindow(xaml_window_internal* internal) : QMainWindow(), m_internal(internal) {}

private:
    void resizeEvent(QResizeEvent* event) override
    {
        m_internal->on_resize_event(event);
    }

    void moveEvent(QMoveEvent* event) override
    {
        m_internal->on_move_event(event);
    }

    void closeEvent(QCloseEvent* event) override
    {
        m_internal->on_close_event(event);
    }
};

xaml_window_internal::~xaml_window_internal() {}

xaml_result xaml_window_internal::draw(xaml_rectangle const&) noexcept
{
    if (!m_handle)
    {
        m_handle.reset(new XamlMainWindow(this));
        xaml_ptr<xaml_application> app;
        XAML_RETURN_IF_FAILED(xaml_application_current(&app));
        XAML_RETURN_IF_FAILED(app->window_added(static_cast<xaml_window*>(m_outer_this)));
        XAML_RETURN_IF_FAILED(draw_resizable());
        XAML_RETURN_IF_FAILED(draw_title());
    }
    XAML_RETURN_IF_FAILED(draw_size());
    XAML_RETURN_IF_FAILED(draw_menu_bar());
    XAML_RETURN_IF_FAILED(draw_child());
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_size() noexcept
{
    return xaml_control_internal::draw_size();
}

xaml_result xaml_window_internal::draw_title() noexcept
{
    QString title;
    XAML_RETURN_IF_FAILED(to_QString(m_title, &title));
    m_handle->setWindowTitle(title);
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_child() noexcept
{
    if (m_child)
    {
        xaml_rectangle region;
        XAML_RETURN_IF_FAILED(get_client_region(&region));
        return m_child->draw(region);
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_resizable() noexcept
{
    auto flags = m_handle->windowFlags();
    if (m_is_resizable)
    {
        flags |= Qt::WindowMaximizeButtonHint;
    }
    else
    {
        flags &= ~Qt::WindowMaximizeButtonHint;
    }
    m_handle->setWindowFlags(flags);
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_menu_bar() noexcept
{
    if (m_menu_bar)
    {
        XAML_RETURN_IF_FAILED(m_menu_bar->set_parent(static_cast<xaml_control*>(m_outer_this)));
        XAML_RETURN_IF_FAILED(m_menu_bar->draw({}));
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::show() noexcept
{
    XAML_RETURN_IF_FAILED(draw({}));
    m_handle->show();
    return XAML_S_OK;
}

xaml_result xaml_window_internal::close() noexcept
{
    m_handle->close();
    return XAML_S_OK;
}

xaml_result xaml_window_internal::hide() noexcept
{
    m_handle->hide();
    return XAML_S_OK;
}

xaml_result xaml_window_internal::get_client_region(xaml_rectangle* pregion) noexcept
{
    *pregion = xaml_from_native(m_handle->geometry());
    return XAML_S_OK;
}

xaml_result xaml_window_internal::get_dpi(double* pvalue) noexcept
{
    *pvalue = m_handle->screen()->logicalDotsPerInch();
    return XAML_S_OK;
}

void xaml_window_internal::on_resize_event(QResizeEvent* event) noexcept
{
    xaml_atomic_guard guard{ m_resizing };
    if (!guard.test_and_set())
    {
        XAML_ASSERT_SUCCEEDED(set_size(xaml_from_native(event->size())));
    }
}

void xaml_window_internal::on_move_event(QMoveEvent* event) noexcept
{
    xaml_atomic_guard guard{ m_resizing };
    if (!guard.test_and_set())
    {
        XAML_ASSERT_SUCCEEDED(set_location(xaml_from_native(event->pos())));
    }
}

void xaml_window_internal::on_close_event(QCloseEvent* event) noexcept
{
    xaml_ptr<xaml_object> handled;
    XAML_ASSERT_SUCCEEDED(xaml_box_value(false, &handled));
    XAML_ASSERT_SUCCEEDED(on_closing(m_outer_this, handled));
    bool value;
    XAML_ASSERT_SUCCEEDED(xaml_unbox_value(handled, &value));
    if (value)
    {
        event->ignore();
    }
    else
    {
        event->accept();
        xaml_ptr<xaml_application> app;
        XAML_ASSERT_SUCCEEDED(xaml_application_current(&app));
        XAML_ASSERT_SUCCEEDED(app->window_removed(static_cast<xaml_window*>(m_outer_this)));
    }
}
