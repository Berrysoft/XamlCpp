#include <QMenu>
#include <qt5/qstring.hpp>
#include <shared/menu_item.hpp>
#include <xaml/ui/qt5/control.hpp>
#include <xaml/ui/qt5/menu_bar.hpp>

xaml_result xaml_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    if (!m_handle)
    {
        if (m_parent)
        {
            xaml_ptr<xaml_qt5_control> native_parnet;
            if (XAML_SUCCEEDED(m_parent->query(&native_parnet)))
            {
                m_handle = native_parnet->get_handle();
            }
        }
    }
    XAML_RETURN_IF_FAILED(draw_append(&m_action));
    return XAML_S_OK;
}

xaml_result xaml_menu_item_internal::draw_append(QAction** ptr) noexcept
{
    QMenu* pmenu = nullptr;
    QMenuBar* pbar = nullptr;
    if (m_parent)
    {
        xaml_ptr<xaml_qt5_menu_bar> native_menu_bar;
        if (XAML_SUCCEEDED(m_parent->query(&native_menu_bar)))
        {
            XAML_RETURN_IF_FAILED(native_menu_bar->get_handle(&pbar));
        }
        else
        {
            xaml_ptr<xaml_qt5_control> native_menu_item;
            if (XAML_SUCCEEDED(m_parent->query(&native_menu_item)))
            {
                pmenu = qobject_cast<QMenu*>(native_menu_item->get_handle().get());
            }
        }
    }

    if (pmenu || pbar)
    {
        if (auto menu = m_handle.objectCast<QMenu>(); menu && menu != pmenu)
        {
            if (pmenu)
                *ptr = pmenu->addMenu(menu.get());
            else
                *ptr = pbar->addMenu(menu.get());
        }
        else
        {
            QString text;
            XAML_RETURN_IF_FAILED(to_QString(m_text, &text));
            if (pmenu)
                *ptr = pmenu->addAction(text, xaml_mem_fn(&xaml_menu_item_internal::on_triggerd, this));
            else
                *ptr = pbar->addAction(text, xaml_mem_fn(&xaml_menu_item_internal::on_triggerd, this));
        }
    }
    return XAML_S_OK;
}

void xaml_menu_item_internal::on_triggerd() noexcept
{
    XAML_ASSERT_SUCCEEDED(on_click(m_outer_this));
}

xaml_result xaml_popup_menu_item_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        auto menu = QSharedPointer<QMenu>(new QMenu);
        m_handle = menu.staticCast<QWidget>();
        QString text;
        XAML_RETURN_IF_FAILED(to_QString(m_text, &text));
        menu->setTitle(text);
        XAML_RETURN_IF_FAILED(xaml_menu_item_internal::draw(region));
        XAML_RETURN_IF_FAILED(draw_submenu());
    }
    XAML_RETURN_IF_FAILED(draw_append(&m_action));
    return XAML_S_OK;
}

xaml_result xaml_popup_menu_item_internal::draw_submenu() noexcept
{
    XAML_FOREACH_START(child, m_submenu);
    {
        xaml_ptr<xaml_control> cc;
        XAML_RETURN_IF_FAILED(child->query(&cc));
        XAML_RETURN_IF_FAILED(cc->draw({}));
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result xaml_check_menu_item_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        XAML_RETURN_IF_FAILED(xaml_menu_item_internal::draw(region));
        m_action->setCheckable(true);
        XAML_RETURN_IF_FAILED(draw_checked());
    }
    return XAML_S_OK;
}

xaml_result xaml_check_menu_item_internal::draw_checked() noexcept
{
    m_action->setChecked(m_is_checked);
    return XAML_S_OK;
}

xaml_result xaml_radio_menu_item_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        XAML_RETURN_IF_FAILED(xaml_menu_item_internal::draw(region));
        m_action->setCheckable(true);
        XAML_RETURN_IF_FAILED(draw_checked());
        XAML_RETURN_IF_FAILED(draw_group());
    }
    return XAML_S_OK;
}

xaml_result xaml_radio_menu_item_internal::draw_checked() noexcept
{
    m_action->setChecked(m_is_checked);
    return XAML_S_OK;
}

xaml_result xaml_separator_menu_item_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        XAML_RETURN_IF_FAILED(xaml_menu_item_internal::draw(region));
        m_action->setSeparator(true);
    }
    return XAML_S_OK;
}
