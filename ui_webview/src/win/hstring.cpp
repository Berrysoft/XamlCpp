#include <win/hstring.hpp>

using namespace std;
using namespace winrt;

struct xaml_string_hstring_impl : xaml_implement<xaml_string_hstring_impl, xaml_string, xaml_object>
{
    hstring m_str;

    xaml_string_hstring_impl(hstring const& str) noexcept : m_str(str) {}

    xaml_result XAML_CALL get_length(int32_t* psize) noexcept override
    {
        *psize = (int32_t)m_str.size();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_data(xaml_char_t const** ptr) noexcept override
    {
        if (m_str.empty())
            *ptr = nullptr;
        else
            *ptr = m_str.data();
        return XAML_S_OK;
    }

    xaml_result XAML_CALL equals(xaml_string* str, bool* pres) noexcept override
    {
        int32_t rhs_length;
        XAML_RETURN_IF_FAILED(str->get_length(&rhs_length));
        xaml_char_t const* rhs_data;
        XAML_RETURN_IF_FAILED(str->get_data(&rhs_data));
        if ((int32_t)m_str.size() != rhs_length)
        {
            *pres = false;
        }
        else
        {
            *pres = char_traits<xaml_char_t>::compare(m_str.data(), rhs_data, m_str.size()) == 0;
        }
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_string_new_hstring(winrt::hstring const& str, xaml_string** ptr) noexcept
{
    return xaml_object_new<xaml_string_hstring_impl>(ptr, str);
}
