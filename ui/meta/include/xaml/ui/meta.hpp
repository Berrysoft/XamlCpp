#ifndef XAML_UI_META_HPP
#define XAML_UI_META_HPP

#include <xaml/meta/meta.hpp>
#include <xaml/utility.hpp>

namespace xaml
{
    struct ui_init_traits
    {
        XAML_UI_META_API static void init_meta(std::shared_ptr<meta_context> const& ctx = nullptr) noexcept;
    };
} // namespace xaml

#endif // !XAML_UI_META_HPP
