#ifndef XAML_UI_CANVAS_GTK3_PEN_H
#define XAML_UI_CANVAS_GTK3_PEN_H

#include <cairo.h>
#include <xaml/ui/controls/pen.h>

XAML_CLASS(xaml_gtk3_pen, { 0x422b5e58, 0x42cf, 0x43e4, { 0xa4, 0x86, 0x1f, 0x63, 0xa8, 0xdb, 0x35, 0xf5 } })

#define XAML_GTK3_PEN_VTBL(type)               \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(set, type, cairo_t*)

XAML_DECL_INTERFACE_(xaml_gtk3_pen, xaml_object)
{
    XAML_DECL_VTBL(xaml_gtk3_pen, XAML_GTK3_PEN_VTBL);
};

#endif // !XAML_UI_CANVAS_GTK3_PEN_H
