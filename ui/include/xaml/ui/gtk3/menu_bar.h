#ifndef XAML_UI_GTK3_MENU_BAR_H
#define XAML_UI_GTK3_MENU_BAR_H

#include <gtk/gtk.h>
#include <xaml/ui/control.h>

XAML_CLASS(xaml_gtk3_menu_bar, { 0xe04ca998, 0x56bd, 0x45bd, { 0xa3, 0x87, 0x4e, 0x80, 0x5b, 0xcd, 0x8c, 0xe3 } })

#define XAML_GTK3_MENU_BAR_VTBL(type)          \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(handle, type, GtkMenuBar**, GtkMenuBar*)

XAML_DECL_INTERFACE_(xaml_gtk3_menu_bar, xaml_object)
{
    XAML_DECL_VTBL(xaml_gtk3_menu_bar, XAML_GTK3_MENU_BAR_VTBL);
};

#endif // !XAML_UI_GTK3_MENU_BAR_H
