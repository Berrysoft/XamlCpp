#ifndef XAML_UI_CANVAS_BRUSH_H
#define XAML_UI_CANVAS_BRUSH_H

#include <xaml/meta/meta_macros.h>
#include <xaml/ui/drawing.h>

XAML_CLASS(xaml_brush, { 0x6b543544, 0x961a, 0x4096, { 0xb3, 0x7b, 0x2c, 0x79, 0xaa, 0xbc, 0xd4, 0x39 } })

#define XAML_BRUSH_VTBL(type) XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type))

XAML_DECL_INTERFACE_(xaml_brush, xaml_object)
{
    XAML_DECL_VTBL(xaml_brush, XAML_BRUSH_VTBL);
};

XAML_CLASS(xaml_solid_brush, { 0x4a1295f2, 0xe5b9, 0x4bb2, { 0xa5, 0x90, 0x78, 0x59, 0xaf, 0x59, 0x8d, 0x9e } })

#define XAML_SOLID_BRUSH_VTBL(type)           \
    XAML_VTBL_INHERIT(XAML_BRUSH_VTBL(type)); \
    XAML_PROP(color, type, xaml_color*, xaml_color)

XAML_DECL_INTERFACE_(xaml_solid_brush, xaml_brush)
{
    XAML_DECL_VTBL(xaml_solid_brush, XAML_SOLID_BRUSH_VTBL);
};

EXTERN_C XAML_UI_CANVAS_API xaml_result XAML_CALL xaml_solid_brush_new(xaml_color, xaml_solid_brush**) XAML_NOEXCEPT;

#endif // !XAML_UI_CANVAS_BRUSH_H
