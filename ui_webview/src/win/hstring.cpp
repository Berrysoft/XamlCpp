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

    xaml_result XAML_CALL get_data(char const** ptr) noexcept override
    {
        if (m_str.empty())
            *ptr = nullptr;
        else
            *ptr = m_str.data();
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_string_new_hstring(winrt::hstring const& str, xaml_string** ptr) noexcept
{
    return xaml_object_new<xaml_string_hstring_impl>(ptr, str);
}
