#ifndef XAML_INTERNAL_GTK3_RESOURCES_HPP
#define XAML_INTERNAL_GTK3_RESOURCES_HPP

#include <gtk/gtk.h>
#include <memory>

namespace xaml
{
    template <typename T = void, void (*Deleter)(T*) = ::g_free>
    struct g_free_deleter
    {
        void operator()(void* ptr) const noexcept { Deleter((T*)ptr); }
    };

    template <typename T>
    using g_free_unique_ptr = std::unique_ptr<T, g_free_deleter<>>;

    template <typename T>
    using g_slist_free_unique_ptr = std::unique_ptr<T, g_free_deleter<::GSList, ::g_slist_free>>;
} // namespace xaml

#endif // !XAML_INTERNAL_GTK3_RESOURCES_HPP
