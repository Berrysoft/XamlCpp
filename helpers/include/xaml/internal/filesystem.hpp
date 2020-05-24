#ifndef XAML_INTERNAL_FILESYSTEM_HPP
#define XAML_INTERNAL_FILESYSTEM_HPP

#include <filesystem>
#include <string_view>
#include <xaml/string.h>

inline std::filesystem::path to_path(std::string_view str)
{
#ifdef XAML_WIN32
    return to_wstring(str);
#else
    return str;
#endif // XAML_WIN32
}

inline xaml_result to_path(std::string_view str, std::filesystem::path* pvalue) noexcept
try
{
    *pvalue = to_path(str);
    return XAML_S_OK;
}
XAML_CATCH_RETURN()

inline std::filesystem::path to_path(xaml_ptr<xaml_string> const& str)
{
    return to_path(to_string_view(str));
}

inline xaml_result to_path(xaml_ptr<xaml_string> const& str, std::filesystem::path* pvalue) noexcept
{
    std::string_view view;
    XAML_RETURN_IF_FAILED(to_string_view(str, &view));
    return to_path(str, pvalue);
}

#endif // !XAML_INTERNAL_FILESYSTEM_HPP
