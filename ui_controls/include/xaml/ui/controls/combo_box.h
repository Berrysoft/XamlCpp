#ifndef XAML_UI_CONTROLS_COMBO_BOX_H
#define XAML_UI_CONTROLS_COMBO_BOX_H

#include <xaml/ui/controls/items_base.h>

XAML_CLASS(xaml_combo_box, { 0x804bf4c0, 0xff2b, 0x445d, { 0x99, 0x83, 0x4b, 0xb2, 0x0d, 0x85, 0xa4, 0x09 } })

#define XAML_COMBO_BOX_VTBL(type)                             \
    XAML_VTBL_INHERIT(XAML_ITEMS_BASE_VTBL(type));            \
    XAML_PROP(text, type, xaml_string**, xaml_string*);       \
    XAML_EVENT(text_changed, type, xaml_object, xaml_string); \
    XAML_PROP(is_editable, type, bool*, bool);                \
    XAML_EVENT(is_editable_changed, type, xaml_object, bool)

XAML_DECL_INTERFACE_(xaml_combo_box, xaml_items_base)
{
    XAML_DECL_VTBL(xaml_combo_box, XAML_COMBO_BOX_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_combo_box_new(xaml_combo_box**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_combo_box_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_combo_box_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_COMBO_BOX_H
