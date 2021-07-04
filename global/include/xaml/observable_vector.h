#ifndef XAML_OBSERVABLE_VECTOR_H
#define XAML_OBSERVABLE_VECTOR_H

#include <xaml/delegate.h>
#include <xaml/event.h>
#include <xaml/vector.h>

typedef enum xaml_vector_changed_action
{
    xaml_vector_changed_add,
    xaml_vector_changed_erase,
    xaml_vector_changed_replace,
    xaml_vector_changed_move,
    xaml_vector_changed_reset
} xaml_vector_changed_action;

XAML_TYPE_BASE(xaml_vector_changed_args_1, { 0xf081fd5b, 0xd6d3, 0x4262, { 0xa7, 0xc7, 0x5d, 0x25, 0x82, 0x2d, 0x3b, 0x0a } })

#define XAML_VECTOR_CHANGED_ARGS_1_VTBL(type, TN, TI)                \
    XAML_VTBL_INHERIT(XAML_EVENT_ARGS_VTBL(type));                   \
    XAML_METHOD(get_action, type, xaml_vector_changed_action*);      \
    XAML_METHOD(get_new_items, type, XAML_VECTOR_VIEW_1_NAME(TN)**); \
    XAML_METHOD(get_new_index, type, XAML_STD int32_t*);             \
    XAML_METHOD(get_old_items, type, XAML_VECTOR_VIEW_1_NAME(TN)**); \
    XAML_METHOD(get_old_index, type, XAML_STD int32_t*)

#ifdef __cplusplus
XAML_DECL_INTERFACE_T_(xaml_vector_changed_args, xaml_event_args, XAML_VECTOR_CHANGED_ARGS_1_VTBL)

    #define XAML_VECTOR_CHANGED_ARGS_1_NAME(type) xaml_vector_changed_args<type>

    #define __XAML_VECTOR_CHANGED_ARGS_1_TYPE(type) typedef xaml_vector_changed_args<type> xaml_vector_changed_args_1__##type;
#else
    #define XAML_VECTOR_CHANGED_ARGS_1_NAME(type) xaml_vector_changed_args_1__##type

    #define __XAML_VECTOR_CHANGED_ARGS_1_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_vector_changed_args_1, type_name, XAML_VECTOR_CHANGED_ARGS_1_VTBL, type_name, type_interface)
#endif // __cplusplus
#define XAML_VECTOR_CHANGED_ARGS_1_TYPE(type) __XAML_VECTOR_CHANGED_ARGS_1_TYPE(type)

#ifdef __cplusplus
template <typename T>
struct __xaml_vector_changed_args_implement : xaml_implement<__xaml_vector_changed_args_implement<T>, xaml_vector_changed_args<T>>
{
    xaml_vector_changed_action m_action;
    xaml_ptr<xaml_vector_view<T>> m_new_items;
    std::int32_t m_new_index;
    xaml_ptr<xaml_vector_view<T>> m_old_items;
    std::int32_t m_old_index;

    __xaml_vector_changed_args_implement(xaml_vector_changed_action action, xaml_vector_view<T>* new_items, std::int32_t new_index, xaml_vector_view<T>* old_items, std::int32_t old_index) noexcept
        : m_action(action), m_new_items(new_items), m_new_index(new_index), m_old_items(old_items), m_old_index(old_index) {}

    xaml_result XAML_CALL get_action(xaml_vector_changed_action* paction) noexcept override
    {
        *paction = m_action;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_new_items(xaml_vector_view<T>** ptr) noexcept override
    {
        return m_new_items.query(ptr);
    }

    xaml_result XAML_CALL get_new_index(std::int32_t* pindex) noexcept override
    {
        *pindex = m_new_index;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_old_items(xaml_vector_view<T>** ptr) noexcept override
    {
        return m_old_items.query(ptr);
    }

    xaml_result XAML_CALL get_old_index(std::int32_t* pindex) noexcept override
    {
        *pindex = m_old_index;
        return XAML_S_OK;
    }
};

template <typename T>
xaml_result XAML_CALL xaml_vector_changed_args_new(xaml_vector_changed_action action, xaml_vector_view<T>* new_items, std::int32_t new_index, xaml_vector_view<T>* old_items, std::int32_t old_index, xaml_vector_changed_args<T>** ptr) noexcept
{
    return xaml_object_new<__xaml_vector_changed_args_implement<T>>(ptr, action, new_items, new_index, old_items, old_index);
}
#endif // __cplusplus

XAML_TYPE_BASE(xaml_observable_vector_1, { 0xc84cb35f, 0x0a1c, 0x40e2, { 0x8e, 0x1c, 0x2c, 0x43, 0x0b, 0x1b, 0xb6, 0xcf } })

#ifndef __XAML_DELEGATE_2_NAME
    #define __XAML_DELEGATE_2_NAME(a, b) XAML_DELEGATE_2_NAME(a, b)
#endif // !__XAML_DELEGATE_2_NAME

#define XAML_OBSERVABLE_VECTOR_1_VTBL(type, TN, TI)                                                                                      \
    XAML_VTBL_INHERIT(XAML_VECTOR_1_VTBL(type, TN, TI));                                                                                 \
    XAML_METHOD(add_vector_changed, type, __XAML_DELEGATE_2_NAME(xaml_object, XAML_VECTOR_CHANGED_ARGS_1_NAME(TN))*, XAML_STD int32_t*); \
    XAML_METHOD(remove_vector_changed, type, XAML_STD int32_t)

#ifdef __cplusplus
XAML_DECL_INTERFACE_T_(xaml_observable_vector, xaml_vector<T>, XAML_OBSERVABLE_VECTOR_1_VTBL)

    #define XAML_OBSERVABLE_VECTOR_1_NAME(type) xaml_observable_vector<type>

    #define __XAML_OBSERVABLE_VECTOR_1_TYPE(type) typedef xaml_observable_vector<type> xaml_observable_vector_1__##type;
#else
    #define XAML_OBSERVABLE_VECTOR_1_NAME(type) xaml_observable_vector_1__##type

    #define __XAML_OBSERVABLE_VECTOR_1_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_observable_vector_1, type_name, XAML_OBSERVABLE_VECTOR_1_VTBL, type_name, type_interface)
#endif // __cplusplus
#define XAML_OBSERVABLE_VECTOR_1_TYPE(type) __XAML_OBSERVABLE_VECTOR_1_TYPE(type)

#ifdef __cplusplus
template <typename T>
struct __xaml_observable_vector_implement : xaml_implement<__xaml_observable_vector_implement<T>, xaml_observable_vector<T>>
{
    xaml_ptr<xaml_vector<T>> m_vec{};
    xaml_ptr<xaml_event<xaml_object, xaml_vector_changed_args<T>>> m_collection_changed{};

    xaml_result XAML_CALL on_collection_changed(xaml_vector_changed_action action, xaml_vector_view<T>* new_items, std::int32_t new_index, xaml_vector_view<T>* old_items, std::int32_t old_index) noexcept
    {
        xaml_ptr<xaml_vector_changed_args<T>> args;
        XAML_RETURN_IF_FAILED(xaml_vector_changed_args_new<T>(action, new_items, new_index, old_items, old_index, &args));
        return m_collection_changed->invoke(this, args);
    }

    xaml_result XAML_CALL init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_vec));
        XAML_RETURN_IF_FAILED(xaml_event_new(&m_collection_changed));
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_size(std::int32_t* psize) noexcept override
    {
        return m_vec->get_size(psize);
    }

    xaml_result XAML_CALL index_of(xaml_interface_t<T> value, std::int32_t* pindex) noexcept override
    {
        return m_vec->index_of(value, pindex);
    }

    xaml_result XAML_CALL get_at(std::int32_t index, xaml_interface_t<T>* ptr) noexcept override
    {
        return m_vec->get_at(index, ptr);
    }

    xaml_result XAML_CALL set_at(int32_t index, xaml_interface_t<T> obj) noexcept override
    {
        xaml_interface_var_t<T> old_item;
        XAML_RETURN_IF_FAILED(m_vec->get_at(index, &old_item));
        XAML_RETURN_IF_FAILED(m_vec->set_at(index, obj));
        xaml_ptr<xaml_vector<T>> old_items, new_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new<T>({ old_item }, &old_items));
        XAML_RETURN_IF_FAILED(xaml_vector_new<T>({ obj }, &new_items));
        return on_collection_changed(xaml_vector_changed_replace, new_items, index, old_items, index);
    }

    xaml_result XAML_CALL append(xaml_interface_t<T> obj) noexcept override
    {
        std::int32_t index;
        XAML_RETURN_IF_FAILED(m_vec->get_size(&index));
        XAML_RETURN_IF_FAILED(m_vec->append(obj));
        xaml_ptr<xaml_vector<T>> new_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new<T>({ obj }, &new_items));
        return on_collection_changed(xaml_vector_changed_add, new_items, index, nullptr, index);
    }

    xaml_result XAML_CALL insert_at(std::int32_t index, xaml_interface_t<T> obj) noexcept override
    {
        XAML_RETURN_IF_FAILED(m_vec->insert_at(index, obj));
        xaml_ptr<xaml_vector<T>> new_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new<T>({ obj }, &new_items));
        return on_collection_changed(xaml_vector_changed_add, new_items, index, nullptr, index);
    }

    xaml_result XAML_CALL remove_at(std::int32_t index) noexcept override
    {
        xaml_interface_var_t<T> old_item;
        XAML_RETURN_IF_FAILED(m_vec->get_at(index, &old_item));
        XAML_RETURN_IF_FAILED(m_vec->remove_at(index));
        xaml_ptr<xaml_vector<T>> old_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new<T>({ old_item }, &old_items));
        return on_collection_changed(xaml_vector_changed_erase, nullptr, index, old_items, index);
    }

    xaml_result XAML_CALL remove_at_end() noexcept override
    {
        std::int32_t index;
        XAML_RETURN_IF_FAILED(m_vec->get_size(&index));
        index--;
        xaml_interface_var_t<T> old_item;
        XAML_RETURN_IF_FAILED(m_vec->get_at(index, &old_item));
        XAML_RETURN_IF_FAILED(m_vec->remove_at_end());
        xaml_ptr<xaml_vector<T>> old_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new<T>({ old_item }, &old_items));
        return on_collection_changed(xaml_vector_changed_erase, nullptr, index, old_items, index);
    }

    xaml_result XAML_CALL clear() noexcept override
    {
        xaml_ptr<xaml_vector<T>> old_items = m_vec;
        m_vec = nullptr;
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_vec));
        return on_collection_changed(xaml_vector_changed_erase, nullptr, 0, old_items, 0);
    }

    xaml_result XAML_CALL get_enumerator(xaml_enumerator<T>** ptr) noexcept override
    {
        return m_vec->get_enumerator(ptr);
    }

    xaml_result XAML_CALL add_vector_changed(xaml_delegate<xaml_object, xaml_vector_changed_args<T>>* handler, std::int32_t* ptoken) noexcept override
    {
        return m_collection_changed->add(handler, ptoken);
    }

    xaml_result XAML_CALL remove_vector_changed(int32_t token) noexcept override
    {
        return m_collection_changed->remove(token);
    }
};

template <typename T>
xaml_result XAML_CALL xaml_observable_vector_new(xaml_observable_vector<T>** ptr) noexcept
{
    return xaml_object_init_catch<__xaml_observable_vector_implement<T>>(ptr);
}
#endif // __cplusplus

#endif // !XAML_OBSERVABLE_VECTOR_H
