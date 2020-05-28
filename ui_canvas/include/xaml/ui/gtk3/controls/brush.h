#ifndef XAML_UI_CANVAS_GTK3_BRUSH_H
#define XAML_UI_CANVAS_GTK3_BRUSH_H

#include "xaml/object.h"
#include <cairo.h>
#include <xaml/ui/controls/brush.h>

XAML_CLASS(xaml_gtk3_brush, { 0xb9c7f548, 0x9703, 0x438c, { 0xb8, 0x62, 0x8b, 0x9d, 0x7e, 0xab, 0x97, 0xed } })

#define XAML_GTK3_BRUSH_VTBL(type)             \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(set, type, cairo_t*)

XAML_DECL_INTERFACE_(xaml_gtk3_brush, xaml_object)
{
    XAML_DECL_VTBL(xaml_gtk3_brush, XAML_GTK3_BRUSH_VTBL);
};

#endif // !XAML_UI_CANVAS_GTK3_BRUSH_H
