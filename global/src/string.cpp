#include <algorithm>
#include <xaml/object.h>
#include <xaml/string.h>

using namespace std;

struct xaml_string_impl : xaml_implement<xaml_string_impl, xaml_string, xaml_object>
{
private:
    xaml_std_string_t m_str{};

public:
    xaml_string_impl() noexcept {}
    xaml_string_impl(xaml_char_t const* str) : m_str(str) {}
    xaml_string_impl(xaml_std_string_t&& str) : m_str(move(str)) {}
    xaml_string_impl(xaml_std_string_view_t str) : m_str(str) {}

    xaml_result XAML_CALL get_length(size_t* psize) const noexcept override
    {
        *psize = m_str.size();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_data(xaml_char_t const** ptr) const noexcept override
    {
        if (m_str.empty())
            *ptr = nullptr;
        else
            *ptr = m_str.c_str();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL equals(xaml_string* str, bool* pres) const noexcept override
    {
        size_t rhs_length;
        xaml_result hr = str->get_length(&rhs_length);
        if (XAML_FAILED(hr)) return hr;
        xaml_char_t const* rhs_data;
        hr = str->get_data(&rhs_data);
        if (XAML_FAILED(hr)) return hr;
        *pres = equal(m_str.begin(), m_str.end(), rhs_data, rhs_data + rhs_length, char_traits<xaml_char_t>::eq);
        return XAML_S_OK;
    }
};

xaml_result xaml_string_new(xaml_char_t const* str, xaml_string** ptr) noexcept
{
    if (str)
        return xaml_object_new<xaml_string_impl>(ptr, str);
    else
        return xaml_object_new<xaml_string_impl>(ptr);
}

xaml_result xaml_string_new(xaml_std_string_t&& str, xaml_string** ptr) noexcept
{
    return xaml_object_new<xaml_string_impl>(ptr, move(str));
}

xaml_result xaml_string_new(xaml_std_string_view_t str, xaml_string** ptr) noexcept
{
    return xaml_object_new<xaml_string_impl>(ptr, str);
}
