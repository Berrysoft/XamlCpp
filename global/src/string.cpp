#include <algorithm>
#include <xaml/object.h>
#include <xaml/string.h>

#ifdef XAML_WIN32
    #include <nowide/convert.hpp>
#endif // XAML_WIN32

using namespace std;

template <typename T, typename String>
struct xaml_string_implement : xaml_implement<T, xaml_string>
{
    String m_str{};

    xaml_string_implement() noexcept {}

    xaml_result XAML_CALL get_length(int32_t* psize) noexcept override
    {
        *psize = static_cast<int32_t>(m_str.size());
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

struct xaml_string_impl : xaml_string_implement<xaml_string_impl, std::string>
{
    xaml_string_impl() {}
    xaml_result XAML_CALL init(char const* str)
    {
        try
        {
            m_str = str;
            return XAML_S_OK;
        }
        XAML_CATCH_RETURN()
    }
    xaml_string_impl(std::string&& str) noexcept { m_str = move(str); }
    xaml_result XAML_CALL init(std::string_view str)
    {
        try
        {
            m_str = str;
            return XAML_S_OK;
        }
        XAML_CATCH_RETURN()
    }
};

struct xaml_string_view_impl : xaml_string_implement<xaml_string_view_impl, std::string_view>
{
    xaml_string_view_impl() noexcept {}
    xaml_string_view_impl(char const* str) noexcept { m_str = str; }
    xaml_string_view_impl(std::string_view str) noexcept { m_str = str; }
};

xaml_result XAML_CALL xaml_string_new(char const* str, xaml_string** ptr) noexcept
{
    if (str)
        return xaml_object_init_catch<xaml_string_impl>(ptr, str);
    else
        return xaml_object_new_catch<xaml_string_impl>(ptr);
}

xaml_result XAML_CALL xaml_string_new_length(char const* str, int32_t length, xaml_string** ptr) noexcept
{
    return xaml_string_new(string_view(str, static_cast<size_t>(length)), ptr);
}

xaml_result XAML_CALL xaml_string_new_view(char const* str, xaml_string** ptr) noexcept
{
    if (str)
        return xaml_object_new<xaml_string_view_impl>(ptr, str);
    else
        return xaml_object_new<xaml_string_view_impl>(ptr);
}

xaml_result XAML_CALL xaml_string_new_view_length(char const* str, int32_t length, xaml_string** ptr) noexcept
{
    return xaml_string_new_view(string_view(str, (size_t)length), ptr);
}

xaml_result XAML_CALL xaml_string_new(std::string&& str, xaml_string** ptr) noexcept
{
    return xaml_object_new<xaml_string_impl>(ptr, move(str));
}

xaml_result XAML_CALL xaml_string_new(std::string_view str, xaml_string** ptr) noexcept
{
    return xaml_object_init_catch<xaml_string_impl>(ptr, str);
}

xaml_result XAML_CALL xaml_string_new_view(std::string_view str, xaml_string** ptr) noexcept
{
    if (*(str.data() + str.size())) return XAML_E_INVALIDARG;
    return xaml_object_new<xaml_string_view_impl>(ptr, str);
}

ostream& operator<<(ostream& stream, xaml_ptr<xaml_string> const& str)
{
    return stream << to_string_view(str);
}

#ifdef XAML_WIN32
wstring to_wstring(string_view view)
{
    return nowide::widen(view);
}

pmr::wstring to_pmr_wstring(string_view str, pmr::polymorphic_allocator<wchar_t> alloc)
{
    return nowide::convert<wchar_t>(str, alloc);
}

wstring_view __xaml_codecvt_pool_impl::operator()(string_view view)
{
    size_t len = view.length() + 1;
    wchar_t* result = m_wide_allocator.allocate(len);
    return nowide::widen(result, len, view);
}

string to_string(wstring_view view)
{
    return nowide::narrow(view);
}

pmr::string to_pmr_string(wstring_view wstr, pmr::polymorphic_allocator<char> alloc)
{
    return nowide::convert<char>(wstr, alloc);
}

string_view __xaml_codecvt_pool_impl::operator()(wstring_view view)
{
    size_t len = view.length() * 2 + 1;
    char* result = m_allocator.allocate(len);
    return nowide::narrow(result, len, view);
}
#endif // XAML_WIN32

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
        std::string_view lhs_view;
        XAML_RETURN_IF_FAILED(to_string_view(lhs, &lhs_view));
        std::string_view rhs_view;
        XAML_RETURN_IF_FAILED(to_string_view(rhs, &rhs_view));
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
    char const* data;
    XAML_RETURN_IF_FAILED(str->get_data(&data));
    return xaml_string_new(data, ptr);
}

xaml_result XAML_CALL xaml_string_concat(xaml_string* lhs, xaml_string* rhs, xaml_string** ptr) noexcept
try
{
    std::string str;
    XAML_RETURN_IF_FAILED(to_string(lhs, &str));
    char const* rhs_data;
    XAML_RETURN_IF_FAILED(rhs->get_data(&rhs_data));
    str += rhs_data;
    return xaml_string_new(move(str), ptr);
}
XAML_CATCH_RETURN()

xaml_result XAML_CALL xaml_string_substr(xaml_string* str, int32_t offset, int32_t length, xaml_string** ptr) noexcept
{
    if (offset < 0) return XAML_E_OUTOFBOUNDS;
    std::string_view view;
    XAML_RETURN_IF_FAILED(to_string_view(str, &view));
    return xaml_string_new(view.substr(static_cast<size_t>(offset), static_cast<size_t>(length)), ptr);
}
