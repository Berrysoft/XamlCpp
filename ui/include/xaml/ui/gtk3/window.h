#ifndef XAML_UI_GTK3_WINDOW_H
#define XAML_UI_GTK3_WINDOW_H

#include <xaml/ui/gtk3/control.h>

XAML_CLASS(xaml_gtk3_window, { 0xd062e63e, 0x6c5a, 0x4543, { 0xa7, 0x47, 0x1f, 0x31, 0x19, 0x7d, 0xfb, 0x82 } })

#define XAML_GTK3_WINDOW_VTBL(type)                   \
    XAML_VTBL_INHERIT(XAML_GTK3_CONTROL_VTBL(type));  \
    XAML_PROP(window, type, GtkWidget**, GtkWidget*); \
    XAML_PROP(vbox, type, GtkWidget**, GtkWidget*);   \
    XAML_PROP(menu_bar, type, GtkWidget**, GtkWidget*)

XAML_DECL_INTERFACE_(xaml_gtk3_window, xaml_gtk3_control)
{
    XAML_DECL_VTBL(xaml_gtk3_window, XAML_GTK3_WINDOW_VTBL);
};

#endif // !XAML_UI_GTK3_WINDOW_H
