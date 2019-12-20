#ifndef XAML_UI_META_HPP
#define XAML_UI_META_HPP

#include <xaml/ui/application.hpp>
#include <xaml/ui/button.hpp>
#include <xaml/ui/grid.hpp>
#include <xaml/ui/window.hpp>

namespace xaml
{
    inline void init_meta() noexcept
    {
        register_class<
            application,
            window,
            grid,
            button>();
    }
} // namespace xaml

#endif // !XAML_UI_META_HPP
