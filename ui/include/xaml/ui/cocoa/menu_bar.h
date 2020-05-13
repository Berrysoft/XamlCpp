#ifndef XAML_UI_COCOA_MENU_BAR_H
#define XAML_UI_COCOA_MENU_BAR_H

#include <xaml/meta/meta_macros.h>
#include <xaml/object.h>
#include <xaml/ui/cocoa/objc.h>

XAML_CLASS(xaml_cocoa_menu_bar, { 0xe04ca998, 0x56bd, 0x45bd, { 0xa3, 0x87, 0x4e, 0x80, 0x5b, 0xcd, 0x8c, 0xe3 } })

#define XAML_COCOA_MENU_BAR_VTBL(type)         \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(handle, type, OBJC_OBJECT(NSMenu)*, OBJC_OBJECT(NSMenu))

XAML_DECL_INTERFACE_(xaml_cocoa_menu_bar, xaml_object)
{
    XAML_DECL_VTBL(xaml_cocoa_menu_bar, XAML_COCOA_MENU_BAR_VTBL);
};

#endif // !XAML_UI_COCOA_MENU_BAR_H
