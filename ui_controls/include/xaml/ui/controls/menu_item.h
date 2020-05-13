#ifndef XAML_UI_CONTROLS_MENU_ITEM_H
#define XAML_UI_CONTROLS_MENU_ITEM_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_menu_item, { 0xbaa16b2b, 0x1cca, 0x4a7c, { 0x8b, 0xbe, 0x96, 0x2b, 0x56, 0x95, 0x67, 0x7f } })

#define XAML_MENU_ITEM_VTBL(type)                       \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));         \
    XAML_PROP(text, type, xaml_string**, xaml_string*); \
    XAML_EVENT(click, type)

XAML_DECL_INTERFACE_(xaml_menu_item, xaml_control)
{
    XAML_DECL_VTBL(xaml_menu_item, XAML_MENU_ITEM_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_menu_item_new(xaml_menu_item**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_menu_item_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_menu_item_register(xaml_meta_context*) XAML_NOEXCEPT;

XAML_CLASS(xaml_popup_menu_item, { 0xc1445529, 0x6932, 0x46a0, { 0xb9, 0x47, 0x2f, 0xe0, 0x04, 0x63, 0xa1, 0x27 } })

#define XAML_POPUP_MENU_ITEM_VTBL(type)                 \
    XAML_VTBL_INHERIT(XAML_MENU_ITEM_VTBL(type));       \
    XAML_METHOD(get_submenu, type, xaml_vector_view**); \
    XAML_CPROP(submenu, type, xaml_menu_item*, xaml_menu_item*)

XAML_DECL_INTERFACE_(xaml_popup_menu_item, xaml_menu_item)
{
    XAML_DECL_VTBL(xaml_popup_menu_item, XAML_POPUP_MENU_ITEM_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_popup_menu_item_new(xaml_popup_menu_item**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_popup_menu_item_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_popup_menu_item_register(xaml_meta_context*) XAML_NOEXCEPT;

XAML_CLASS(xaml_check_menu_item, { 0xba31cca6, 0x5bb1, 0x4b90, { 0xab, 0x1a, 0x39, 0xcf, 0x13, 0x27, 0x6b, 0x2e } })

#define XAML_CHECK_MENU_ITEM_VTBL(type)           \
    XAML_VTBL_INHERIT(XAML_MENU_ITEM_VTBL(type)); \
    XAML_PROP(is_checked, type, bool*, bool);     \
    XAML_EVENT(is_checked_changed, type)

XAML_DECL_INTERFACE_(xaml_check_menu_item, xaml_menu_item)
{
    XAML_DECL_VTBL(xaml_check_menu_item, XAML_CHECK_MENU_ITEM_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_check_menu_item_new(xaml_check_menu_item**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_check_menu_item_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_check_menu_item_register(xaml_meta_context*) XAML_NOEXCEPT;

XAML_CLASS(xaml_radio_menu_item, { 0x49494137, 0x05e4, 0x477f, { 0xa5, 0x2a, 0xef, 0x07, 0x4e, 0xca, 0x9c, 0x20 } })

#define XAML_RADIO_MENU_ITEM_VTBL(type)           \
    XAML_VTBL_INHERIT(XAML_MENU_ITEM_VTBL(type)); \
    XAML_PROP(is_checked, type, bool*, bool);     \
    XAML_EVENT(is_checked_changed, type);         \
    XAML_PROP(group, type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_radio_menu_item, xaml_menu_item)
{
    XAML_DECL_VTBL(xaml_radio_menu_item, XAML_RADIO_MENU_ITEM_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_radio_menu_item_new(xaml_radio_menu_item**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_radio_menu_item_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_radio_menu_item_register(xaml_meta_context*) XAML_NOEXCEPT;

XAML_CLASS(xaml_separator_menu_item, { 0xecc1f5ab, 0x5420, 0x45bb, { 0x8f, 0xb0, 0x2f, 0xa1, 0xea, 0xbb, 0xec, 0x24 } })

#define XAML_SEPARATOR_MENU_ITEM_VTBL(type) XAML_VTBL_INHERIT(XAML_MENU_ITEM_VTBL(type))

XAML_DECL_INTERFACE_(xaml_separator_menu_item, xaml_menu_item)
{
    XAML_DECL_VTBL(xaml_separator_menu_item, XAML_SEPARATOR_MENU_ITEM_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_separator_menu_item_new(xaml_separator_menu_item**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_separator_menu_item_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_separator_menu_item_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_MENU_ITEM_H
