#ifndef XAML_UI_CONTROLS_ENTRY_H
#define XAML_UI_CONTROLS_ENTRY_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_entry, { 0x483715fa, 0x1124, 0x4d4f, { 0x9e, 0x34, 0x36, 0xeb, 0xc4, 0x78, 0x0b, 0x24 } })

#define XAML_ENTRY_VTBL(type)                           \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));         \
    XAML_PROP(text, type, xaml_string**, xaml_string*); \
    XAML_EVENT(text_changed, type);                     \
    XAML_PROP(text_halignment, type, xaml_halignment*, xaml_halignment)

XAML_DECL_INTERFACE_(xaml_entry, xaml_control)
{
    XAML_DECL_VTBL(xaml_entry, XAML_ENTRY_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_entry_new(xaml_entry**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_entry_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_entry_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_ENTRY_H
