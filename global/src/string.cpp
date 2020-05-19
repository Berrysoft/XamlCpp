#ifdef UNICODE
#ifdef XAML_USE_BOOST_NOWIDE
#include <boost/nowide/convert.hpp>
#else
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <codecvt>
#endif // XAML_USE_BOOST_NOWIDE
#endif // UNICODE

#include <algorithm>
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
#ifdef XAML_USE_BOOST_NOWIDE
static xaml_result to_wstring(string_view str, wstring* pvalue) noexcept
try
{
    *pvalue = boost::nowide::widen(str.data(), str.size());
    return XAML_S_OK;
}
XAML_CATCH_RETURN()

string to_string(xaml_ptr<xaml_string> const& str)
{
    xaml_char_t const* data;
    XAML_THROW_IF_FAILED(str->get_data(&data));
    return boost::nowide::narrow(data);
}
#else
static wstring_convert<codecvt_utf8_utf16<wchar_t>> s_conv;

static xaml_result to_wstring(string_view str, wstring* pvalue) noexcept
try
{
    *pvalue = s_conv.from_bytes(str.data(), str.data() + str.length());
    return XAML_S_OK;
}
XAML_CATCH_RETURN()

string to_string(xaml_ptr<xaml_string> const& str)
{
    xaml_char_t const* data;
    XAML_THROW_IF_FAILED(str->get_data(&data));
    return s_conv.to_bytes(data);
}
#endif // XAML_USE_BOOST_NOWIDE

ostream& operator<<(ostream& stream, xaml_ptr<xaml_string> const& str)
{
    return stream << to_string(str);
}

basic_ostream<xaml_char_t>& operator<<(basic_ostream<xaml_char_t>& stream, xaml_ptr<xaml_string> const& str)
{
    return stream << to_string_view_t(str);
}

xaml_result XAML_CALL xaml_string_new_utf8(char const* str, xaml_string** ptr) noexcept
{
    wstring value;
    XAML_RETURN_IF_FAILED(to_wstring(str, &value));
    return xaml_string_new(move(value), ptr);
}

xaml_result XAML_CALL xaml_string_new_utf8(string_view str, xaml_string** ptr) noexcept
{
    wstring value;
    XAML_RETURN_IF_FAILED(to_wstring(str, &value));
    return xaml_string_new(move(value), ptr);
}
#else
string to_string(xaml_ptr<xaml_string> const& str)
{
    return to_string_t(str);
}

ostream& operator<<(ostream& stream, xaml_ptr<xaml_string> const& str)
{
    return stream << to_string_view_t(str);
}

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

xaml_result XAML_CALL xaml_string_empty(xaml_string** ptr) noexcept
{
    static xaml_string_view_impl s_empty_string{};
    s_empty_string.add_ref();
    *ptr = &s_empty_string;
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_string_equals(xaml_string* lhs, xaml_string* rhs, bool* pres) noexcept
{
    if (lhs && rhs)
    {
        xaml_std_string_view_t lhs_view;
        XAML_RETURN_IF_FAILED(to_string_view_t(lhs, &lhs_view));
        xaml_std_string_view_t rhs_view;
        XAML_RETURN_IF_FAILED(to_string_view_t(rhs, &rhs_view));
        *pres = lhs_view == rhs_view;
    }
    else
    {
        *pres = !lhs && !rhs;
    }
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_string_clone(xaml_string* str, xaml_string** ptr) noexcept
{
    xaml_char_t const* data;
    XAML_RETURN_IF_FAILED(str->get_data(&data));
    return xaml_string_new(data, ptr);
}

xaml_result XAML_CALL xaml_string_concat(xaml_string* lhs, xaml_string* rhs, xaml_string** ptr) noexcept
try
{
    xaml_std_string_t str;
    XAML_RETURN_IF_FAILED(to_string_t(lhs, &str));
    xaml_char_t const* rhs_data;
    XAML_RETURN_IF_FAILED(rhs->get_data(&rhs_data));
    str += rhs_data;
    return xaml_string_new(move(str), ptr);
}
XAML_CATCH_RETURN()

xaml_result XAML_CALL xaml_string_substr(xaml_string* str, int32_t offset, int32_t length, xaml_string** ptr) noexcept
{
    if (offset < 0) return XAML_E_OUTOFBOUNDS;
    xaml_std_string_view_t view;
    XAML_RETURN_IF_FAILED(to_string_view_t(str, &view));
    return xaml_string_new(view.substr((size_t)offset, (size_t)length), ptr);
}
