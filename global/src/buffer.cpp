#include <vector>
#include <xaml/buffer.h>

using namespace std;

struct xaml_buffer_impl : xaml_implement<xaml_buffer_impl, xaml_buffer, xaml_object>
{
private:
    vector<uint8_t> m_buffer;

public:
    xaml_buffer_impl(int32_t size) noexcept : m_buffer(static_cast<size_t>(size)) {}
    xaml_buffer_impl(vector<uint8_t>&& buffer) noexcept : m_buffer(move(buffer)) {}

    xaml_result XAML_CALL get_size(int32_t* psize) noexcept override
    {
        *psize = static_cast<int32_t>(m_buffer.size());
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_data(uint8_t** pdata) noexcept override
    {
        *pdata = m_buffer.data();
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_buffer_new(int32_t size, xaml_buffer** ptr) noexcept
{
    return xaml_object_new<xaml_buffer_impl>(ptr, size);
}

xaml_result XAML_CALL xaml_buffer_new(vector<uint8_t>&& vec, xaml_buffer** ptr) noexcept
{
    return xaml_object_new<xaml_buffer_impl>(ptr, move(vec));
}

struct xaml_buffer_reference_impl : xaml_implement<xaml_buffer_reference_impl, xaml_buffer, xaml_object>
{
private:
    uint8_t* m_data;
    int32_t m_size;

public:
    xaml_buffer_reference_impl(uint8_t* data, int32_t size) noexcept : m_data(data), m_size(size) {}
    xaml_buffer_reference_impl(vector<uint8_t>& buffer) noexcept : m_data(buffer.data()), m_size(static_cast<int32_t>(buffer.size())) {}

    xaml_result XAML_CALL get_size(int32_t* psize) noexcept override
    {
        *psize = m_size;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_data(uint8_t** pdata) noexcept override
    {
        *pdata = m_data;
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_buffer_new_reference(uint8_t* data, int32_t size, xaml_buffer** ptr) noexcept
{
    return xaml_object_new<xaml_buffer_reference_impl>(ptr, data, size);
}

xaml_result XAML_CALL xaml_buffer_new_reference(vector<uint8_t>& vec, xaml_buffer** ptr) noexcept
{
    return xaml_object_new<xaml_buffer_reference_impl>(ptr, vec);
}
