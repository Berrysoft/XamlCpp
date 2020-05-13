#ifndef XAML_UI_CONTROLS_RADIO_BOX_H
#define XAML_UI_CONTROLS_RADIO_BOX_H

#include <xaml/ui/controls/button.h>

XAML_CLASS(xaml_radio_box, { 0x90dbaa0c, 0x93fe, 0x4934, { 0x88, 0x58, 0xb7, 0xe2, 0xfb, 0x54, 0xa6, 0x53 } })

#define XAML_RADIO_BOX_VTBL(type)              \
    XAML_VTBL_INHERIT(XAML_BUTTON_VTBL(type)); \
    XAML_PROP(is_checked, type, bool*, bool);  \
    XAML_EVENT(is_checked_changed, type);      \
    XAML_PROP(group, type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_radio_box, xaml_button)
{
    XAML_DECL_VTBL(xaml_radio_box, XAML_RADIO_BOX_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_radio_box_new(xaml_radio_box**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_radio_box_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_radio_box_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_RADIO_BOX_H
