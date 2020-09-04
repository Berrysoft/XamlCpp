#include <xaml/event.h>
#include <xaml/object.h>
#include <xaml/observable_vector.h>
#include <xaml/ptr.hpp>
#include <xaml/vector.h>

using namespace std;

struct xaml_observable_vector_impl : xaml_implement<xaml_observable_vector_impl, xaml_observable_vector>
{
private:
    xaml_ptr<xaml_vector> m_vec{ nullptr };
    xaml_ptr<xaml_event> m_collection_changed{ nullptr };

    xaml_result XAML_CALL on_collection_changed(xaml_vector_changed_action action, xaml_vector_view* new_items, int32_t new_index, xaml_vector_view* old_items, int32_t old_index) noexcept
    {
        xaml_ptr<xaml_vector_changed_args> args;
        XAML_RETURN_IF_FAILED(xaml_vector_changed_args_new(action, new_items, new_index, old_items, old_index, &args));
        xaml_ptr<xaml_vector> invoke_args;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ this, args }, &invoke_args));
        xaml_ptr<xaml_object> obj;
        return m_collection_changed->invoke(invoke_args, &obj);
    }

public:
    xaml_result XAML_CALL init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_vec));
        XAML_RETURN_IF_FAILED(xaml_event_new(&m_collection_changed));
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_size(int32_t* psize) noexcept override
    {
        return m_vec->get_size(psize);
    }

    xaml_result XAML_CALL index_of(xaml_object* value, int32_t* pindex) noexcept override
    {
        return m_vec->index_of(value, pindex);
    }

    xaml_result XAML_CALL get_at(int32_t index, xaml_object** ptr) noexcept override
    {
        return m_vec->get_at(index, ptr);
    }

    xaml_result XAML_CALL set_at(int32_t index, xaml_object* obj) noexcept override
    {
        xaml_ptr<xaml_object> old_item;
        XAML_RETURN_IF_FAILED(m_vec->get_at(index, &old_item));
        XAML_RETURN_IF_FAILED(m_vec->set_at(index, obj));
        xaml_ptr<xaml_vector> old_items, new_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ old_item }, &old_items));
        XAML_RETURN_IF_FAILED(xaml_vector_new({ obj }, &new_items));
        return on_collection_changed(xaml_vector_changed_replace, new_items, index, old_items, index);
    }

    xaml_result XAML_CALL append(xaml_object* obj) noexcept override
    {
        int32_t index;
        XAML_RETURN_IF_FAILED(m_vec->get_size(&index));
        XAML_RETURN_IF_FAILED(m_vec->append(obj));
        xaml_ptr<xaml_vector> new_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ obj }, &new_items));
        return on_collection_changed(xaml_vector_changed_add, new_items, index, nullptr, index);
    }

    xaml_result XAML_CALL insert_at(int32_t index, xaml_object* obj) noexcept override
    {
        XAML_RETURN_IF_FAILED(m_vec->insert_at(index, obj));
        xaml_ptr<xaml_vector> new_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ obj }, &new_items));
        return on_collection_changed(xaml_vector_changed_add, new_items, index, nullptr, index);
    }

    xaml_result XAML_CALL remove_at(int32_t index) noexcept override
    {
        xaml_ptr<xaml_object> old_item;
        XAML_RETURN_IF_FAILED(m_vec->get_at(index, &old_item));
        XAML_RETURN_IF_FAILED(m_vec->remove_at(index));
        xaml_ptr<xaml_vector> old_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ old_item }, &old_items));
        return on_collection_changed(xaml_vector_changed_erase, nullptr, index, old_items, index);
    }

    xaml_result XAML_CALL remove_at_end() noexcept override
    {
        int32_t index;
        XAML_RETURN_IF_FAILED(m_vec->get_size(&index));
        index--;
        xaml_ptr<xaml_object> old_item;
        XAML_RETURN_IF_FAILED(m_vec->get_at(index, &old_item));
        XAML_RETURN_IF_FAILED(m_vec->remove_at_end());
        xaml_ptr<xaml_vector> old_items;
        XAML_RETURN_IF_FAILED(xaml_vector_new({ old_item }, &old_items));
        return on_collection_changed(xaml_vector_changed_erase, nullptr, index, old_items, index);
    }

    xaml_result XAML_CALL clear() noexcept override
    {
        xaml_ptr<xaml_vector> old_items = m_vec;
        m_vec = nullptr;
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_vec));
        return on_collection_changed(xaml_vector_changed_erase, nullptr, 0, old_items, 0);
    }

    xaml_result XAML_CALL get_enumerator(xaml_enumerator** ptr) noexcept override
    {
        return m_vec->get_enumerator(ptr);
    }

    xaml_result XAML_CALL add_vector_changed(xaml_delegate* handler, int32_t* ptoken) noexcept override
    {
        return m_collection_changed->add(handler, ptoken);
    }

    xaml_result XAML_CALL remove_vector_changed(int32_t token) noexcept override
    {
        return m_collection_changed->remove(token);
    }
};

xaml_result XAML_CALL xaml_observable_vector_new(xaml_observable_vector** ptr) noexcept
{
    return xaml_object_init<xaml_observable_vector_impl>(ptr);
}

struct xaml_vector_changed_args_impl : xaml_implement<xaml_vector_changed_args_impl, xaml_vector_changed_args>
{
private:
    xaml_vector_changed_action m_action;
    xaml_ptr<xaml_vector_view> m_new_items;
    int32_t m_new_index;
    xaml_ptr<xaml_vector_view> m_old_items;
    int32_t m_old_index;

public:
    xaml_vector_changed_args_impl(xaml_vector_changed_action action, xaml_vector_view* new_items, int32_t new_index, xaml_vector_view* old_items, int32_t old_index) noexcept
        : m_action(action), m_new_items(new_items), m_new_index(new_index), m_old_items(old_items), m_old_index(old_index) {}

    xaml_result XAML_CALL get_action(xaml_vector_changed_action* paction) noexcept override
    {
        *paction = m_action;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_new_items(xaml_vector_view** ptr) noexcept override
    {
        return m_new_items->query(ptr);
    }

    xaml_result XAML_CALL get_new_index(int32_t* pindex) noexcept override
    {
        *pindex = m_new_index;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_old_items(xaml_vector_view** ptr) noexcept override
    {
        return m_old_items->query(ptr);
    }

    xaml_result XAML_CALL get_old_index(int32_t* pindex) noexcept override
    {
        *pindex = m_old_index;
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_vector_changed_args_new(xaml_vector_changed_action action, xaml_vector_view* new_items, int32_t new_index, xaml_vector_view* old_items, int32_t old_index, xaml_vector_changed_args** ptr) noexcept
{
    return xaml_object_new<xaml_vector_changed_args_impl>(ptr, action, new_items, new_index, old_items, old_index);
}
