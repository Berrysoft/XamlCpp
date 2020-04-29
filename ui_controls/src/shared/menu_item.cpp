#include <shared/menu_item.hpp>
#include <xaml/ui/controls/menu_item.h>
#include <xaml/ui/menu_bar.h>

using namespace std;

xaml_result xaml_popup_menu_item_impl::add_submenu(xaml_menu_item* child) noexcept
{
    if (child)
    {
        child->set_parent(this);
        XAML_RETURN_IF_FAILED(m_submenu->append(child));
        XAML_RETURN_IF_FAILED(parent_redraw());
    }
    return XAML_S_OK;
}

xaml_result xaml_popup_menu_item_impl::remove_submenu(xaml_menu_item* child) noexcept
{
    return XAML_E_NOTIMPL;
}

xaml_result xaml_check_menu_item_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_menu_item_implement::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_checked_changed));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_is_checked_changed->add_noexcept<xaml_ptr<xaml_check_menu_item>, bool>(
        [this](xaml_ptr<xaml_check_menu_item>, bool) -> xaml_result {
            if (m_menu_id) XAML_RETURN_IF_FAILED(draw_checked());
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

xaml_result xaml_radio_menu_item_impl::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_menu_item_implement::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_checked_changed));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_is_checked_changed->add_noexcept<xaml_ptr<xaml_radio_menu_item>, bool>(
        [this](xaml_ptr<xaml_radio_menu_item>, bool) -> xaml_result {
            if (m_menu_id)
            {
                XAML_RETURN_IF_FAILED(draw_checked());
                XAML_RETURN_IF_FAILED(draw_group());
            }
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

#ifndef XAML_UI_GTK3
xaml_result xaml_radio_menu_item_impl::draw_group() noexcept
{
    if (m_parent && m_is_checked)
    {
        xaml_ptr<xaml_menu_bar> multic;
        if (XAML_SUCCEEDED(m_parent->query(&multic)))
        {
            xaml_ptr<xaml_vector_view> children;
            XAML_RETURN_IF_FAILED(multic->get_children(&children));
            XAML_FOREACH_START(c, children);
            {
                if (auto rc = c.query<xaml_radio_menu_item>())
                {
                    if (rc.get() != this)
                    {
                        xaml_ptr<xaml_string> group;
                        XAML_RETURN_IF_FAILED(rc->get_group(&group));
                        bool equals;
                        XAML_RETURN_IF_FAILED(group->equals(m_group.get(), &equals));
                        if (equals)
                        {
                            XAML_RETURN_IF_FAILED(rc->set_is_checked(false));
                        }
                    }
                }
            }
            XAML_FOREACH_END();
        }
    }
    return XAML_S_OK;
}
#endif // !XAML_UI_GTK3
