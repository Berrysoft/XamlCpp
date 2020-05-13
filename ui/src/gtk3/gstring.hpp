#ifndef XAML_UI_GTK3_GSTRING_HPP
#define XAML_UI_GTK3_GSTRING_HPP

#include <gtk3/resources.hpp>
#include <xaml/string.h>

xaml_result XAML_CALL xaml_string_new_gstring(g_free_unique_ptr<gchar>&&, xaml_string**) noexcept;

#endif // !XAML_UI_GTK3_GSTRING_HPP
