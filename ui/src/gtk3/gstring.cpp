#include <gtk3/gstring.hpp>

using namespace std;

struct xaml_string_hstring_impl : xaml_implement<xaml_string_hstring_impl, xaml_string, xaml_object>
{
    g_free_unique_ptr<gchar> m_str;

    xaml_string_hstring_impl(g_free_unique_ptr<gchar>&& str) noexcept : m_str(move(str)) {}

    xaml_result XAML_CALL get_length(int32_t* psize) noexcept override
    {
        *psize = (int32_t)char_traits<gchar>::length(m_str.get());
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_data(xaml_char_t const** ptr) noexcept override
    {
        if (!m_str.get()[0])
            *ptr = nullptr;
        else
            *ptr = m_str.get();
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_string_new_gstring(g_free_unique_ptr<gchar>&& str, xaml_string** ptr) noexcept
{
    return xaml_object_new<xaml_string_hstring_impl>(ptr, move(str));
}
