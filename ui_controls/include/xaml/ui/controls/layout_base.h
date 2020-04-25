#ifndef XAML_UI_CONTROLS_LAYOUT_BASE_H
#define XAML_UI_CONTROLS_LAYOUT_BASE_H

#include <xaml/ui/container.h>

typedef enum xaml_orientation
{
    xaml_orientation_horizontal,
    xaml_orientation_vertical
} xaml_orientation;

XAML_TYPE(xaml_orientation, { 0xbf0f301a, 0xeac7, 0x45a8, { 0x8c, 0xf2, 0x95, 0x69, 0x83, 0xea, 0x18, 0xf3 } })

XAML_CLASS(xaml_layout_base, { 0x11aee394, 0x31f8, 0x40ac, { 0xb3, 0x8f, 0x69, 0xb8, 0x86, 0x36, 0x68, 0x22 } })

#define XAML_LAYOUT_BASE_VTBL(type) XAML_VTBL_INHERIT(XAML_MULTICONTAINER_VTBL(type))

XAML_DECL_INTERFACE_(xaml_layout_base, xaml_multicontainer)
{
    XAML_DECL_VTBL(xaml_layout_base, XAML_LAYOUT_BASE_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result xaml_layout_base_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result xaml_layout_base_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_LAYOUT_BASE_H
