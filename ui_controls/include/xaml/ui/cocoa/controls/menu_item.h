#ifndef XAML_UI_CONTROLS_COCOA_MENU_ITEM_H
#define XAML_UI_CONTROLS_COCOA_MENU_ITEM_H

#include <xaml/meta/meta_macros.h>
#include <xaml/ui/cocoa/objc.h>

XAML_CLASS(xaml_cocoa_menu_item, { 0x51502d01, 0x46e2, 0x4828, { 0x81, 0x86, 0x97, 0x80, 0x8c, 0x6e, 0x49, 0x04 } })

#define XAML_COCOA_MENU_ITEM_VTBL(type)        \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(menu, type, OBJC_OBJECT(NSMenuItem)*, OBJC_OBJECT(NSMenuItem))

XAML_DECL_INTERFACE_(xaml_cocoa_menu_item, xaml_object)
{
    XAML_DECL_VTBL(xaml_cocoa_menu_item, XAML_COCOA_MENU_ITEM_VTBL);
};

XAML_CLASS(xaml_cocoa_popup_menu_item, { 0x9aadd946, 0xb5ee, 0x4f1c, { 0xbc, 0x8e, 0x63, 0x24, 0x9a, 0xbb, 0x31, 0x95 } })

#define XAML_COCOA_POPUP_MENU_ITEM_VTBL(type)           \
    XAML_VTBL_INHERIT(XAML_COCOA_MENU_ITEM_VTBL(type)); \
    XAML_PROP(handle, type, OBJC_OBJECT(NSMenu)*, OBJC_OBJECT(NSMenu))

XAML_DECL_INTERFACE_(xaml_cocoa_popup_menu_item, xaml_cocoa_menu_item)
{
    XAML_DECL_VTBL(xaml_cocoa_popup_menu_item, XAML_COCOA_POPUP_MENU_ITEM_VTBL);
};

#endif // !XAML_UI_CONTROLS_COCOA_MENU_ITEM_H
