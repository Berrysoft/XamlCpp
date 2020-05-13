#ifndef XAML_UI_GTK3_CONTROL_H
#define XAML_UI_GTK3_CONTROL_H

#include <gtk/gtk.h>
#include <xaml/ui/control.h>

XAML_CLASS(xaml_gtk3_control, { 0x5ecf49d1, 0x2729, 0x4eca, { 0x9e, 0xc7, 0xc3, 0x7d, 0xc8, 0xef, 0xb8, 0x6b } })

#define XAML_GTK3_CONTROL_VTBL(type)           \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(handle, type, GtkWidget**, GtkWidget*)

XAML_DECL_INTERFACE_(xaml_gtk3_control, xaml_object)
{
    XAML_DECL_VTBL(xaml_gtk3_control, XAML_GTK3_CONTROL_VTBL);
};

#endif // !XAML_UI_GTK3_CONTROL_H
