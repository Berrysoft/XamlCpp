#include <xaml/box.h>
#include <xaml/object.hpp>

using namespace std;

struct xaml_box_impl : xaml_implement<xaml_box_impl, xaml_box, xaml_object>
{
private:
    xaml_guid m_type;
    void* m_data;
    size_t m_size;

public:
    xaml_box_impl(xaml_guid const& type, void* data, size_t size) noexcept : m_type(type), m_data(data), m_size(size) {}
    ~xaml_box_impl() { free(m_data); }

    xaml_result XAML_CALL get_type(xaml_guid* ptype) const noexcept override
    {
        *ptype = m_type;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_data(void const** pdata) const noexcept override
    {
        *pdata = m_data;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_size(size_t* psize) const noexcept override
    {
        *psize = m_size;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_data(xaml_guid const& type, void const* data, size_t size) noexcept override
    {
        if (m_size != size)
        {
            void* new_data = realloc(m_data, size);
            if (!new_data) return XAML_E_OUTOFMEMORY;
            m_data = new_data;
        }
        m_type = type;
        m_size = size;
        memcpy(m_data, data, size);
        return XAML_S_OK;
    }
};

xaml_result xaml_box_new(xaml_guid const& type, void const* data, size_t size, xaml_box** ptr) noexcept
{
    void* storage = malloc(size);
    if (!storage) return XAML_E_OUTOFMEMORY;
    memcpy(storage, data, size);
    return xaml_object_new<xaml_box_impl>(ptr, type, storage, size);
}
