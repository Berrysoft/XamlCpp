#ifndef XAML_UI_WINDOW_HPP
#define XAML_UI_WINDOW_HPP

#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class window : public control, meta_class_impl<window>
    {
    };
} // namespace xaml

#endif // !XAML_UI_WINDOW_HPP
