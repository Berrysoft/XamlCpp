#ifndef XAML_WEBVIEW_HSTRING_HPP
#define XAML_WEBVIEW_HSTRING_HPP

#include <xaml/string.h>

#include "winrt/base.h"

xaml_result XAML_CALL xaml_string_new_hstring(winrt::hstring const&, xaml_string**) noexcept;

#endif // !XAML_WEBVIEW_HSTRING_HPP
