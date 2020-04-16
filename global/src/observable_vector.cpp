#include <xaml/event.h>
#include <xaml/object.hpp>
#include <xaml/observable_vector.h>
#include <xaml/xaml_ptr.hpp>

using namespace std;

struct xaml_observable_vector_impl : xaml_implement<xaml_observable_vector_impl, xaml_observable_vector, xaml_vector, xaml_vector_view, xaml_enumerable, xaml_object>
{
private:
    xaml_ptr<xaml_vector> m_vec;
    xaml_ptr<xaml_event> m_collection_changed;

public:
    xaml_observable_vector_impl(xaml_vector* vec, xaml_event* ccevent) noexcept : m_vec(vec), m_collection_changed(ccevent) {}

    xaml_result XAML_CALL get_size(size_t* psize) const noexcept override
    {
        return m_vec->get_size(psize);
    }

    xaml_result XAML_CALL get_at(size_t index, xaml_object** ptr) const noexcept override
    {
        return m_vec->get_at(index, ptr);
    }

    xaml_result XAML_CALL set_at(size_t index, xaml_object* obj) noexcept override
    {
        xaml_result hr = m_vec->set_at(index, obj);
        if (XAML_SUCCESS(hr))
        {
            // TODO
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL append(xaml_object* obj) noexcept override
    {
        xaml_result hr = m_vec->append(obj);
        if (XAML_SUCCESS(hr))
        {
            // TODO
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL remove_at(size_t index) noexcept override
    {
        xaml_result hr = m_vec->remove_at(index);
        if (XAML_SUCCESS(hr))
        {
            // TODO
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_enumerator(xaml_enumerator** ptr) const noexcept override
    {
        return m_vec->get_enumerator(ptr);
    }

    xaml_result XAML_CALL add_vector_changed(xaml_callback* handler, size_t* ptoken) noexcept override
    {
        return m_collection_changed->add(handler, ptoken);
    }

    xaml_result XAML_CALL remove_vector_changed(size_t token) noexcept override
    {
        return m_collection_changed->remove(token);
    }
};

xaml_result xaml_observable_vector_new(xaml_observable_vector** ptr) noexcept
{
    xaml_vector* vec;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&vec));
    xaml_event* ccevent;
    XAML_RETURN_IF_FAILED(xaml_event_new(&ccevent));
    return xaml_object_new<xaml_observable_vector_impl>(ptr, vec, ccevent);
}
