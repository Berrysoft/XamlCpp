#include <algorithm>
#include <codecvt>
#include <xaml/object.h>
#include <xaml/string.h>

using namespace std;

template <typename T, typename String>
struct xaml_string_implement : xaml_implement<T, xaml_string, xaml_object>
{
    String m_str{};

    xaml_string_implement() noexcept {}

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
        *pres = equal(m_str.begin(), m_str.end(), rhs_data, rhs_data + rhs_length, char_traits<xaml_char_t>::eq);
        return XAML_S_OK;
    }
};

struct xaml_string_impl : xaml_string_implement<xaml_string_impl, xaml_std_string_t>
{
    xaml_string_impl() noexcept {}
    xaml_result XAML_CALL init(xaml_char_t const* str)
    {
        try
        {
            m_str = str;
            return XAML_S_OK;
        }
        XAML_CATCH_RETURN()
    }
    xaml_string_impl(xaml_std_string_t&& str) noexcept { m_str = move(str); }
    xaml_result XAML_CALL init(xaml_std_string_view_t str)
    {
        try
        {
            m_str = str;
            return XAML_S_OK;
        }
        XAML_CATCH_RETURN()
    }
};

struct xaml_string_view_impl : xaml_string_implement<xaml_string_view_impl, xaml_std_string_view_t>
{
    xaml_string_view_impl() noexcept {}
    xaml_string_view_impl(xaml_char_t const* str) noexcept { m_str = str; }
    xaml_string_view_impl(xaml_std_string_view_t str) noexcept { m_str = str; }
};

xaml_result XAML_CALL xaml_string_new(xaml_char_t const* str, xaml_string** ptr) noexcept
{
    if (str)
        return xaml_object_init<xaml_string_impl>(ptr, str);
    else
        return xaml_object_new<xaml_string_impl>(ptr);
}

xaml_result XAML_CALL xaml_string_new_view(xaml_char_t const* str, xaml_string** ptr) noexcept
{
    if (str)
        return xaml_object_new<xaml_string_view_impl>(ptr, str);
    else
        return xaml_object_new<xaml_string_view_impl>(ptr);
}

xaml_result XAML_CALL xaml_string_new(xaml_std_string_t&& str, xaml_string** ptr) noexcept
{
    return xaml_object_new<xaml_string_impl>(ptr, move(str));
}

xaml_result XAML_CALL xaml_string_new(xaml_std_string_view_t str, xaml_string** ptr) noexcept
{
    return xaml_object_init<xaml_string_impl>(ptr, str);
}

xaml_result XAML_CALL xaml_string_new_view(xaml_std_string_view_t str, xaml_string** ptr) noexcept
{
    return xaml_object_new<xaml_string_view_impl>(ptr, str);
}

#ifdef UNICODE
static wstring to_wstring(string_view str) noexcept
{
    wstring_convert<codecvt_utf8_utf16<wchar_t>> conv;
    return conv.from_bytes(str.data(), str.data() + str.length());
}

xaml_result XAML_CALL xaml_string_new_utf8(char const* str, xaml_string** ptr) noexcept
{
    return xaml_string_new(to_wstring(str), ptr);
}

xaml_result XAML_CALL xaml_string_new_utf8(string&& str, xaml_string** ptr) noexcept
{
    return xaml_string_new(to_wstring(move(str)), ptr);
}

xaml_result XAML_CALL xaml_string_new_utf8(string_view str, xaml_string** ptr) noexcept
{
    return xaml_string_new(to_wstring(str), ptr);
}
#else
xaml_result XAML_CALL xaml_string_new_utf8(char const* str, xaml_string** ptr) noexcept
{
    return xaml_string_new(str, ptr);
}

xaml_result XAML_CALL xaml_string_new_utf8(string&& str, xaml_string** ptr) noexcept
{
    return xaml_string_new(move(str), ptr);
}

xaml_result XAML_CALL xaml_string_new_utf8(string_view str, xaml_string** ptr) noexcept
{
    return xaml_string_new(str, ptr);
}
#endif // UNICODE
