#include <xaml/box.h>
#include <xaml/object.h>

using namespace std;

struct xaml_box_impl : xaml_implement<xaml_box_impl, xaml_box, xaml_object>
{
private:
    xaml_guid m_type{};
    void* m_data{};
    int32_t m_size{};

public:
    xaml_result XAML_CALL init(xaml_guid const& type, void const* data, int32_t size)
    {
        m_type = type;
        m_size = size;
        m_data = malloc(size);
        if (!m_data) return XAML_E_OUTOFMEMORY;
        memcpy(m_data, data, size);
        return XAML_S_OK;
    }
    ~xaml_box_impl() { free(m_data); }

    xaml_result XAML_CALL get_type(xaml_guid* ptype) noexcept override
    {
        *ptype = m_type;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_data(void const** pdata) noexcept override
    {
        *pdata = m_data;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_size(int32_t* psize) noexcept override
    {
        *psize = m_size;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_data(xaml_guid const& type, void const* data, int32_t size) noexcept override
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

xaml_result XAML_CALL xaml_box_new(xaml_guid const& type, void const* data, int32_t size, xaml_box** ptr) noexcept
{
    return xaml_object_init<xaml_box_impl>(ptr, type, data, size);
}
