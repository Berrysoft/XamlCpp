#include <QMenu>
#include <qt5/qstring.hpp>
#include <shared/menu_item.hpp>
#include <xaml/ui/qt5/control.hpp>
#include <xaml/ui/qt5/menu_bar.hpp>

using namespace std;

xaml_result xaml_menu_item_internal::draw(xaml_rectangle const&) noexcept
{
    if (!m_handle)
    {
        xaml_ptr<xaml_element_base> parent;
        XAML_RETURN_IF_FAILED(get_parent(&parent));
        if (parent)
        {
            xaml_ptr<xaml_qt5_control> native_parnet;
            if (XAML_SUCCEEDED(parent->query(&native_parnet)))
            {
                XAML_RETURN_IF_FAILED(native_parnet->get_handle(&m_handle));
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
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent)
    {
        xaml_ptr<xaml_qt5_menu_bar> native_menu_bar;
        if (XAML_SUCCEEDED(parent->query(&native_menu_bar)))
        {
            XAML_RETURN_IF_FAILED(native_menu_bar->get_handle(&pbar));
        }
        else
        {
            xaml_ptr<xaml_qt5_control> native_menu_item;
            if (XAML_SUCCEEDED(parent->query(&native_menu_item)))
            {
                QWidget* handle;
                XAML_RETURN_IF_FAILED(native_menu_item->get_handle(&handle));
                pmenu = qobject_cast<QMenu*>(handle);
            }
        }
    }

    if (pmenu || pbar)
    {
        if (auto menu = qobject_cast<QMenu*>(m_handle); menu && menu != pmenu)
        {
            if (pmenu)
                *ptr = pmenu->addMenu(menu);
            else
                *ptr = pbar->addMenu(menu);
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
    xaml_ptr<xaml_event_args> args;
    XAML_ASSERT_SUCCEEDED(xaml_event_args_empty(&args));
    XAML_ASSERT_SUCCEEDED(m_click->invoke(m_outer_this, args));
}

xaml_result xaml_popup_menu_item_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        auto menu = new QMenu();
        m_handle = static_cast<QWidget*>(menu);
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
    XAML_FOREACH_START(xaml_menu_item, cc, m_submenu);
    {
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
