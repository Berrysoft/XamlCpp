#ifndef XAML_UI_CONTROLS_TEXT_BOX_H
#define XAML_UI_CONTROLS_TEXT_BOX_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_text_box, { 0x081aa775, 0x6209, 0x4a2a, { 0xbb, 0x24, 0x78, 0x5b, 0x1f, 0x8e, 0xf8, 0x61 } })

#define XAML_TEXT_BOX_VTBL(type)                        \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));         \
    XAML_PROP(text, type, xaml_string**, xaml_string*); \
    XAML_EVENT(text_changed, type)

XAML_DECL_INTERFACE_(xaml_text_box, xaml_control)
{
    XAML_DECL_VTBL(xaml_text_box, XAML_TEXT_BOX_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_text_box_new(xaml_text_box**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_text_box_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_text_box_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_TEXT_BOX_H
