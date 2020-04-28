#ifndef XAML_UI_CONTROLS_SHARED_ITEMS_BASE_HPP
#define XAML_UI_CONTROLS_SHARED_ITEMS_BASE_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/items_base.h>

template <typename T, typename... Base>
struct xaml_items_base_implement : xaml_control_implement<T, Base..., xaml_items_base>
{
    XAML_EVENT_IMPL(items_changed)
    XAML_PROP_PTR_IMPL_BASE(items, xaml_observable_vector)

    virtual xaml_result XAML_CALL insert_item(std::int32_t index, xaml_ptr<xaml_object> const& value) noexcept = 0;
    virtual xaml_result XAML_CALL remove_item(std::int32_t index) noexcept = 0;
    virtual xaml_result XAML_CALL clear_items() noexcept = 0;
    virtual xaml_result XAML_CALL replace_item(std::int32_t index, xaml_ptr<xaml_object> const& value) = 0;

    std::int32_t m_items_changed_token{ 0 };

    virtual xaml_result XAML_CALL on_items_vector_changed(xaml_ptr<xaml_observable_vector>, xaml_ptr<xaml_vector_changed_args> args) noexcept
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
                XAML_RETURN_IF_FAILED(insert_item(i, item));
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
            for (std::int32_t i = 0; i < size; i++)
            {
                XAML_RETURN_IF_FAILED(remove_item(i + old_index));
            }
            break;
        }
        case xaml_vector_changed_replace:
        {
            xaml_ptr<xaml_vector_view> new_items;
            XAML_RETURN_IF_FAILED(args->get_new_items(&new_items));
            xaml_ptr<xaml_object> item;
            XAML_RETURN_IF_FAILED(new_items->get_at(0, &items));
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

    xaml_result XAML_CALL set_items(xaml_observable_vector* value) noexcept override
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
                XAML_RETURN_IF_FAILED((xaml_delegate_new_noexcept<xaml_ptr<xaml_observable_vector>, xaml_ptr<xaml_vector_changed_args>>([this](xaml_ptr<xaml_observable_vector> sender, xaml_ptr<xaml_vector_changed_args> args) -> xaml_result { return on_items_vector_changed(sender, args); })));
                XAML_RETURN_IF_FAILED(m_items->add_vector_changed(callback.get(), &m_items_changed_token));
                XAML_RETURN_IF_FAILED(on_items_changed(this, m_items));
            }
        }
        return XAML_S_OK;
    }

    XAML_EVENT_IMPL(sel_id_changed)
    XAML_PROP_EVENT_IMPL(sel_id, std::int32_t, std::int32_t*, std::int32_t)

    xaml_result XAML_CALL init() noexcept override
    {
        XAML_RETURN_IF_FAILED(xaml_control_implement::init());
        XAML_RETURN_IF_FAILED(xaml_event_new(&m_items_changed));
        XAML_RETURN_IF_FAILED(xaml_event_new(&m_sel_id_changed));
        return XAML_S_OK;
    }
};

#endif // !XAML_UI_CONTROLS_SHARED_ITEMS_BASE_HPP
