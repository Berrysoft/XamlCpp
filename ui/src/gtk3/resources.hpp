#ifndef XAML_INTERNAL_GTK3_RESOURCES_HPP
#define XAML_INTERNAL_GTK3_RESOURCES_HPP

#include <gtk/gtk.h>
#include <memory>

template <typename T = void, void (*Deleter)(T*) = ::g_free>
struct g_free_deleter
{
    void operator()(void* ptr) const noexcept { Deleter((T*)ptr); }
};

template <typename T>
using g_free_unique_ptr = std::unique_ptr<T, g_free_deleter<>>;

template <typename T>
using g_object_unique_ptr = std::unique_ptr<T, g_free_deleter<void, ::g_object_unref>>;

using g_list_free_unique_ptr = std::unique_ptr<::GList, g_free_deleter<::GList, ::g_list_free>>;

using g_slist_free_unique_ptr = std::unique_ptr<::GSList, g_free_deleter<::GSList, ::g_slist_free>>;

#endif // !XAML_INTERNAL_GTK3_RESOURCES_HPP
