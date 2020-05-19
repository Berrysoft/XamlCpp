#include <shared/items_base.hpp>
#include <xaml/ui/controls/items_base.h>

xaml_result xaml_items_base_internal::on_items_vector_changed(xaml_ptr<xaml_observable_vector>, xaml_ptr<xaml_vector_changed_args> args) noexcept
{
    xaml_vector_changed_action action;
    XAML_RETURN_IF_FAILED(args->get_action(&action));
    switch (action)
    {
    case xaml_vector_changed_reset:
        XAML_RETURN_IF_FAILED(clear_items());
        [[fallthrough]];
    case xaml_vector_changed_add:
    {
        xaml_ptr<xaml_vector_view> new_items;
        XAML_RETURN_IF_FAILED(args->get_new_items(&new_items));
        std::int32_t size;
        XAML_RETURN_IF_FAILED(new_items->get_size(&size));
        std::int32_t new_index;
        XAML_RETURN_IF_FAILED(args->get_new_index(&new_index));
        for (std::int32_t i = 0; i < size; i++)
        {
            xaml_ptr<xaml_object> item;
            XAML_RETURN_IF_FAILED(new_items->get_at(i, &item));
            XAML_RETURN_IF_FAILED(insert_item(i + new_index, item));
        }
        break;
    }
    case xaml_vector_changed_erase:
    {
        xaml_ptr<xaml_vector_view> old_items;
        XAML_RETURN_IF_FAILED(args->get_old_items(&old_items));
        std::int32_t size;
        XAML_RETURN_IF_FAILED(old_items->get_size(&size));
        std::int32_t old_index;
        XAML_RETURN_IF_FAILED(args->get_old_index(&old_index));
        for (std::int32_t i = old_index; i < size + old_index; i++)
        {
            XAML_RETURN_IF_FAILED(remove_item(i));
        }
        break;
    }
    case xaml_vector_changed_replace:
    {
        xaml_ptr<xaml_vector_view> new_items;
        XAML_RETURN_IF_FAILED(args->get_new_items(&new_items));
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(new_items->get_at(0, &item));
        std::int32_t old_index;
        XAML_RETURN_IF_FAILED(args->get_old_index(&old_index));
        XAML_RETURN_IF_FAILED(replace_item(old_index, item));
        break;
    }
    case xaml_vector_changed_move:
    {
        xaml_ptr<xaml_vector_view> new_items;
        XAML_RETURN_IF_FAILED(args->get_new_items(&new_items));
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(new_items->get_at(0, &item));
        std::int32_t old_index, new_index;
        XAML_RETURN_IF_FAILED(args->get_old_index(&old_index));
        XAML_RETURN_IF_FAILED(args->get_new_index(&new_index));
        XAML_RETURN_IF_FAILED(remove_item(old_index));
        XAML_RETURN_IF_FAILED(insert_item(new_index, item));
        break;
    }
    }
    return XAML_S_OK;
}

xaml_result xaml_items_base_internal::set_items(xaml_observable_vector* value) noexcept
{
    if (m_items.get() != value)
    {
        if (m_items && m_items_changed_token)
        {
            XAML_RETURN_IF_FAILED(m_items->remove_vector_changed(m_items_changed_token));
            m_items_changed_token = 0;
        }
        m_items = value;
        if (m_items)
        {
            xaml_ptr<xaml_delegate> callback;
            XAML_RETURN_IF_FAILED((xaml_delegate_new_noexcept<void, xaml_ptr<xaml_observable_vector>, xaml_ptr<xaml_vector_changed_args>>([this](xaml_ptr<xaml_observable_vector> sender, xaml_ptr<xaml_vector_changed_args> args) -> xaml_result { return on_items_vector_changed(sender, args); }, &callback)));
            XAML_RETURN_IF_FAILED(m_items->add_vector_changed(callback, &m_items_changed_token));
            XAML_RETURN_IF_FAILED(on_items_changed(m_outer_this, m_items));
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_items_base_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_items_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_sel_id_changed));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_items_base_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_items_base;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_PROP_EVENT(items, xaml_observable_vector);
    XAML_TYPE_INFO_ADD_PROP_EVENT(sel_id, int32_t);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_items_base_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_items_base, "xaml/ui/controls/items_base.h");
    XAML_RETURN_IF_FAILED(xaml_items_base_members(__info));
    return ctx->add_type(__info);
}
