#ifndef XAML_INTERNAL_STRING_HPP
#define XAML_INTERNAL_STRING_HPP

#include <nowide/stackstring.hpp>
#include <xaml/string.h>

template <std::size_t BufferSize>
inline xaml_result to_wstring(std::string_view str, nowide::basic_stackstring<wchar_t, char, BufferSize>* wstr) noexcept
try
{
    wstr->convert(str.data());
    return XAML_S_OK;
}
XAML_CATCH_RETURN()

template <std::size_t BufferSize>
inline xaml_result to_wstring(xaml_ptr<xaml_string> const& str, nowide::basic_stackstring<wchar_t, char, BufferSize>* wstr) noexcept
{
    std::string_view view;
    XAML_RETURN_IF_FAILED(to_string_view(str, &view));
    return to_wstring(view, wstr);
}

#endif // !XAML_INTERNAL_STRING_HPP
