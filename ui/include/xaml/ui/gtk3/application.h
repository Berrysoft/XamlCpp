#ifndef XAML_UI_GTK3_APPLICATION_H
#define XAML_UI_GTK3_APPLICATION_H

#include <gtk/gtk.h>
#include <xaml/meta/meta_macros.h>

XAML_CLASS(xaml_gtk3_application, { 0x81f3b45b, 0xbe66, 0x44e0, { 0xad, 0xc3, 0x5b, 0xb9, 0x78, 0xdb, 0x74, 0xc8 } })

#define XAML_GTK3_APPLICATION_VTBL(type)       \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(handle, type, GtkApplication**, GtkApplication*)

XAML_DECL_INTERFACE_(xaml_gtk3_application, xaml_object)
{
    XAML_DECL_VTBL(xaml_gtk3_application, XAML_GTK3_APPLICATION_VTBL);
};

#endif // !XAML_UI_GTK3_APPLICATION_H
