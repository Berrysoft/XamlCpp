#ifndef XAML_UI_WIN32_MENU_BAR_H
#define XAML_UI_WIN32_MENU_BAR_H

#include <Windows.h>
#include <xaml/meta/meta_macros.h>
#include <xaml/object.h>

XAML_CLASS(xaml_win32_menu_bar, { 0xef1c7686, 0xa0ca, 0x442f, { 0x85, 0x27, 0xfa, 0x20, 0xe9, 0xe9, 0x5f, 0x98 } })

#define XAML_WIN32_MENU_BAR_VTBL(type)         \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(handle, type, HMENU*, HMENU)

XAML_DECL_INTERFACE_(xaml_win32_menu_bar, xaml_object)
{
    XAML_DECL_VTBL(xaml_win32_menu_bar, XAML_WIN32_MENU_BAR_VTBL);
};

#endif // !XAML_UI_WIN32_MENU_BAR_H
