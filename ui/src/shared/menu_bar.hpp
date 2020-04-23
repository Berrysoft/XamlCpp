#ifndef XAML_UI_SHARED_MENU_BAR_HPP
#define XAML_UI_SHARED_MENU_BAR_HPP

#include <shared/container.hpp>
#include <xaml/ui/menu_bar.h>
#include <xaml/ui/win/menu_bar.h>

struct xaml_menu_bar_impl : xaml_container_implement<xaml_menu_bar_impl, xaml_menu_bar>
{
    XAML_PROP_IMPL(menu, HMENU, HMENU*, HMENU)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    virtual xaml_result draw_submenu() noexcept;
    xaml_result draw_visible() noexcept override;
};

#endif // !XAML_UI_SHARED_MENU_BAR_HPP
