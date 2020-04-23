#ifndef XAML_UI_MENU_BAR_H
#define XAML_UI_MENU_BAR_H

#include <xaml/ui/container.h>
#include <xaml/ui/window.h>

XAML_CLASS(xaml_menu_bar, { 0x6e6a5fba, 0x5a9c, 0x4771, { 0xb5, 0x2a, 0x83, 0xe7, 0x61, 0xa0, 0x67, 0x46 } })

#define XAML_MENU_BAR_VTBL(type) XAML_VTBL_INHERIT(XAML_MULTICONTAINER_VTBL(type))

XAML_DECL_INTERFACE_(xaml_menu_bar, xaml_multicontainer)
{
    XAML_DECL_VTBL(xaml_menu_bar, XAML_MENU_BAR_VTBL);
};

EXTERN_C XAML_UI_API xaml_result xaml_menu_bar_new(xaml_menu_bar**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result xaml_menu_bar_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result xaml_menu_bar_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_MENU_BAR_H
