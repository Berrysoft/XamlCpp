#ifndef XAML_UI_CONTROLS_LABEL_H
#define XAML_UI_CONTROLS_LABEL_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_label, { 0xa859441c, 0x825c, 0x4682, { 0x8e, 0x04, 0xe2, 0x4d, 0x62, 0x59, 0xe0, 0x87 } })

#define XAML_LABEL_VTBL(type)                                 \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));               \
    XAML_PROP(text, type, xaml_string**, xaml_string*);       \
    XAML_EVENT(text_changed, type, xaml_object, xaml_string); \
    XAML_PROP(text_halignment, type, xaml_halignment*, xaml_halignment)

XAML_DECL_INTERFACE_(xaml_label, xaml_control)
{
    XAML_DECL_VTBL(xaml_label, XAML_LABEL_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_label_new(xaml_label**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_label_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_label_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_LABEL_H
