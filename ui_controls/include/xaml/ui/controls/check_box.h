#ifndef XAML_UI_CONTROLS_CHECK_BOX_H
#define XAML_UI_CONTROLS_CHECK_BOX_H

#include <xaml/ui/controls/button.h>

XAML_CLASS(xaml_check_box, { 0x4d5724c2, 0x77c5, 0x4935, { 0xbd, 0x12, 0x86, 0x86, 0x0d, 0xf6, 0x16, 0xdd } })

#define XAML_CHECK_BOX_VTBL(type)              \
    XAML_VTBL_INHERIT(XAML_BUTTON_VTBL(type)); \
    XAML_PROP(is_checked, type, bool*, bool);  \
    XAML_EVENT(is_check_changed, type)

XAML_DECL_INTERFACE_(xaml_check_box, xaml_button)
{
    XAML_DECL_VTBL(xaml_check_box, XAML_CHECK_BOX_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result xaml_check_box_new(xaml_check_box**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result xaml_check_box_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result xaml_check_box_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_CHECK_BOX_H
