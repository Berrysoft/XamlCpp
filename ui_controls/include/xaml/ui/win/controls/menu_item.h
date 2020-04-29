#ifndef XAML_UI_CONTROLS_WIN_MENU_ITEM_H
#define XAML_UI_CONTROLS_WIN_MENU_ITEM_H

#include <Windows.h>
#include <xaml/object.h>

XAML_CLASS(xaml_win32_menu_item, { 0xefcf6c10, 0xef77, 0x48cf, { 0xa0, 0xc0, 0xbb, 0xfa, 0xdc, 0xb0, 0x05, 0x22 } })

#define XAML_WIN32_MENU_ITEM_VTBL(type)        \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(id, type, UINT*, UINT);          \
    XAML_PROP(parent_handle, type, HMENU*, HMENU)

XAML_DECL_INTERFACE_(xaml_win32_menu_item, xaml_object)
{
    XAML_DECL_VTBL(xaml_win32_menu_item, XAML_WIN32_MENU_ITEM_VTBL);
};

XAML_CLASS(xaml_win32_popup_menu_item, { 0x530d6d9a, 0xccfd, 0x4476, { 0xb5, 0xac, 0xba, 0x06, 0xd0, 0x9b, 0x7d, 0x92 } })

#define XAML_WIN32_POPUP_MENU_ITEM_VTBL(type)           \
    XAML_VTBL_INHERIT(XAML_WIN32_MENU_ITEM_VTBL(type)); \
    XAML_PROP(handle, type, HMENU*, HMENU)

XAML_DECL_INTERFACE_(xaml_win32_popup_menu_item, xaml_win32_menu_item)
{
    XAML_DECL_VTBL(xaml_win32_popup_menu_item, XAML_WIN32_POPUP_MENU_ITEM_VTBL);
};

EXTERN_C xaml_result XAML_CALL xaml_win32_menu_item_generate_id(UINT*) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_WIN_MENU_ITEM_H
