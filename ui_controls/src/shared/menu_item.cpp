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

xaml_result XAML_CALL xaml_menu_item_new(xaml_menu_item** ptr) noexcept
{
    return xaml_object_init<xaml_menu_item_impl>(ptr);
}

xaml_result XAML_CALL xaml_menu_item_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_menu_item;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_menu_item_new);
    XAML_TYPE_INFO_ADD_PROP(text);
    XAML_TYPE_INFO_ADD_EVENT(click);
    XAML_TYPE_INFO_ADD_DEF_PROP(text);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_menu_item_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_menu_item, "xaml/ui/controls/menu_item.h");
    XAML_RETURN_IF_FAILED(xaml_menu_item_members(__info.get()));
    return ctx->add_type(__info.get());
}

xaml_result XAML_CALL xaml_popup_menu_item_new(xaml_popup_menu_item** ptr) noexcept
{
    return xaml_object_init<xaml_popup_menu_item_impl>(ptr);
}

xaml_result XAML_CALL xaml_popup_menu_item_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_popup_menu_item;
    XAML_RETURN_IF_FAILED(xaml_menu_item_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_popup_menu_item_new);
    XAML_TYPE_INFO_ADD_CPROP(submenu);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_popup_menu_item_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_popup_menu_item, "xaml/ui/controls/menu_item.h");
    XAML_RETURN_IF_FAILED(xaml_popup_menu_item_members(__info.get()));
    return ctx->add_type(__info.get());
}

xaml_result XAML_CALL xaml_check_menu_item_new(xaml_check_menu_item** ptr) noexcept
{
    return xaml_object_init<xaml_check_menu_item_impl>(ptr);
}

xaml_result XAML_CALL xaml_check_menu_item_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_check_menu_item;
    XAML_RETURN_IF_FAILED(xaml_menu_item_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_check_menu_item_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_checked);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_check_menu_item_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_check_menu_item, "xaml/ui/controls/menu_item.h");
    XAML_RETURN_IF_FAILED(xaml_check_menu_item_members(__info.get()));
    return ctx->add_type(__info.get());
}

xaml_result XAML_CALL xaml_radio_menu_item_new(xaml_radio_menu_item** ptr) noexcept
{
    return xaml_object_init<xaml_radio_menu_item_impl>(ptr);
}

xaml_result XAML_CALL xaml_radio_menu_item_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_radio_menu_item;
    XAML_RETURN_IF_FAILED(xaml_menu_item_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_radio_menu_item_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_checked);
    XAML_TYPE_INFO_ADD_PROP(group);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_radio_menu_item_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_radio_menu_item, "xaml/ui/controls/menu_item.h");
    XAML_RETURN_IF_FAILED(xaml_radio_menu_item_members(__info.get()));
    return ctx->add_type(__info.get());
}

xaml_result XAML_CALL xaml_separator_menu_item_new(xaml_separator_menu_item** ptr) noexcept
{
    return xaml_object_init<xaml_separator_menu_item_impl>(ptr);
}

xaml_result XAML_CALL xaml_separator_menu_item_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_separator_menu_item;
    XAML_RETURN_IF_FAILED(xaml_menu_item_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_separator_menu_item_new);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_separator_menu_item_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_separator_menu_item, "xaml/ui/controls/menu_item.h");
    XAML_RETURN_IF_FAILED(xaml_separator_menu_item_members(__info.get()));
    return ctx->add_type(__info.get());
}
