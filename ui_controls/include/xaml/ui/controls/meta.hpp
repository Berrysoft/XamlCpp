#ifndef XAML_UI_CONTROLS_META_HPP
#define XAML_UI_CONTROLS_META_HPP

#include <xaml/meta/meta.hpp>
#include <xaml/utility.hpp>

namespace xaml
{
    struct ui_controls_init_traits
    {
        XAML_API static void init_meta(std::shared_ptr<meta_context> const& ctx = nullptr) noexcept;
        XAML_API static void* init_components() noexcept;
        XAML_API static void cleanup_components(void* token) noexcept;
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_META_HPP
