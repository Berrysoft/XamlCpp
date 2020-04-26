#ifndef XAML_UI_CONTROLS_STACK_PANEL_H
#define XAML_UI_CONTROLS_STACK_PANEL_H

#include <xaml/ui/controls/layout_base.h>

XAML_CLASS(xaml_stack_panel, { 0x8cafecf9, 0xd363, 0x463c, { 0xaa, 0x78, 0x8f, 0x39, 0xec, 0x45, 0x53, 0xa5 } })

#define XAML_STACK_PANEL_VTBL(type)                 \
    XAML_VTBL_INHERIT(XAML_LAYOUT_BASE_VTBL(type)); \
    XAML_PROP(orientation, type, xaml_orientation*, xaml_orientation)

XAML_DECL_INTERFACE_(xaml_stack_panel, xaml_layout_base)
{
    XAML_DECL_VTBL(xaml_stack_panel, XAML_STACK_PANEL_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result xaml_stack_panel_new(xaml_stack_panel**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result xaml_stack_panel_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result xaml_stack_panel_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_STACK_PANEL_H
