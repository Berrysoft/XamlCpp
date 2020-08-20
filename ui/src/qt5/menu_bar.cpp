#include <QMainWindow>
#include <shared/menu_bar.hpp>

xaml_result xaml_menu_bar_internal::draw(xaml_rectangle const&) noexcept
{
    auto parent = m_parent;
    xaml_ptr<xaml_qt5_control> native_parent;
    if (XAML_SUCCEEDED(parent->query(&native_parent)))
    {
        QWidget* handle;
        XAML_RETURN_IF_FAILED(native_parent->get_handle(&handle));
        m_handle.reset(handle);
        if (!m_menu)
        {
            if (auto window = qobject_cast<QMainWindow*>(handle))
            {
                m_menu = window->menuBar();
            }
            XAML_RETURN_IF_FAILED(draw_visible());
        }
        XAML_RETURN_IF_FAILED(draw_submenu());
    }
    return XAML_S_OK;
}

xaml_result xaml_menu_bar_internal::draw_submenu() noexcept
{
    XAML_FOREACH_START(child, m_children);
    {
        xaml_ptr<xaml_control> cc;
        XAML_RETURN_IF_FAILED(child->query(&cc));
        XAML_RETURN_IF_FAILED(cc->draw({}));
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_menu_bar_internal::draw_visible() noexcept
{
    m_menu->setVisible(m_is_visible);
    return XAML_S_OK;
}
