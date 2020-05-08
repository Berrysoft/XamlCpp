#ifndef XAML_UI_WIN_STRING_HPP
#define XAML_UI_WIN_STRING_HPP

#include <wil/resource.h>
#include <xaml/string.h>

xaml_result XAML_CALL xaml_string_new_cotaskmem(wil::unique_cotaskmem_string&&, xaml_string**) noexcept;

#endif // !XAML_UI_WIN_STRING_HPP
