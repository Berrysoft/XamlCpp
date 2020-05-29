#ifndef XAML_UI_CANVAS_PEN_H
#define XAML_UI_CANVAS_PEN_H

#include <xaml/meta/meta_macros.h>
#include <xaml/ui/controls/brush.h>
#include <xaml/ui/drawing.h>

XAML_CLASS(xaml_pen, { 0xd8c98fd3, 0x3096, 0x4966, { 0x80, 0xf3, 0x52, 0x19, 0x67, 0xfa, 0x3e, 0x4e } })

#define XAML_PEN_VTBL(type)                    \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(width, type, double*, double)

XAML_DECL_INTERFACE_(xaml_pen, xaml_object)
{
    XAML_DECL_VTBL(xaml_pen, XAML_PEN_VTBL);
};

XAML_CLASS(xaml_brush_pen, { 0xd0044425, 0xe477, 0x412a, { 0x9d, 0x48, 0xaf, 0xa5, 0x0b, 0xfa, 0x62, 0xa0 } })

#define XAML_BRUSH_PEN_VTBL(type)           \
    XAML_VTBL_INHERIT(XAML_PEN_VTBL(type)); \
    XAML_PROP(brush, type, xaml_brush**, xaml_brush*)

XAML_DECL_INTERFACE_(xaml_brush_pen, xaml_pen)
{
    XAML_DECL_VTBL(xaml_brush_pen, XAML_BRUSH_PEN_VTBL);
};

EXTERN_C XAML_UI_CANVAS_API xaml_result XAML_CALL xaml_brush_pen_new(xaml_brush*, double, xaml_brush_pen**) XAML_NOEXCEPT;

#endif // !XAML_UI_CANVAS_PEN_H
