#ifndef XAML_UI_CANVAS_CANVAS_H
#define XAML_UI_CANVAS_CANVAS_H

#include <xaml/ui/control.h>
#include <xaml/ui/drawing.h>

typedef struct xaml_drawing_brush
{
    xaml_color fill;
} xaml_drawing_brush;

typedef struct xaml_drawing_pen
{
    xaml_color stroke;
    double width;
} xaml_drawing_pen;

typedef struct xaml_drawing_font
{
    xaml_char_t const* font_family;
    double size;
    bool italic;
    bool bold;
    xaml_halignment halign;
    xaml_valignment valign;
} xaml_drawing_font;

XAML_CLASS(xaml_drawing_context, { 0xf8b2a81c, 0x11ea, 0x4f0a, { 0x94, 0x97, 0x30, 0x4c, 0x90, 0x65, 0xa4, 0xe4 } })

#define XAML_DRAWING_CONTEXT_VTBL(type)                                                                                             \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                                                                                      \
    XAML_METHOD(draw_arc, type, xaml_drawing_pen XAML_CONST_REF, xaml_rectangle XAML_CONST_REF, double, double);                    \
    XAML_METHOD(fill_pie, type, xaml_drawing_brush XAML_CONST_REF, xaml_rectangle XAML_CONST_REF, double, double);                  \
    XAML_METHOD(draw_ellipse, type, xaml_drawing_pen XAML_CONST_REF, xaml_rectangle XAML_CONST_REF);                                \
    XAML_METHOD(fill_ellipse, type, xaml_drawing_brush XAML_CONST_REF, xaml_rectangle XAML_CONST_REF);                              \
    XAML_METHOD(draw_line, type, xaml_drawing_pen XAML_CONST_REF, xaml_point XAML_CONST_REF, xaml_point XAML_CONST_REF);            \
    XAML_METHOD(draw_rect, type, xaml_drawing_pen XAML_CONST_REF, xaml_rectangle XAML_CONST_REF);                                   \
    XAML_METHOD(fill_rect, type, xaml_drawing_brush XAML_CONST_REF, xaml_rectangle XAML_CONST_REF);                                 \
    XAML_METHOD(draw_round_rect, type, xaml_drawing_pen XAML_CONST_REF, xaml_rectangle XAML_CONST_REF, xaml_size XAML_CONST_REF);   \
    XAML_METHOD(fill_round_rect, type, xaml_drawing_brush XAML_CONST_REF, xaml_rectangle XAML_CONST_REF, xaml_size XAML_CONST_REF); \
    XAML_METHOD(draw_string, type, xaml_drawing_brush XAML_CONST_REF, xaml_drawing_font XAML_CONST_REF, xaml_point XAML_CONST_REF, xaml_string*)

XAML_DECL_INTERFACE_(xaml_drawing_context, xaml_object)
{
    XAML_DECL_VTBL(xaml_drawing_context, XAML_DRAWING_CONTEXT_VTBL);
};

XAML_CLASS(xaml_canvas, { 0x111d5785, 0x4e7e, 0x48c6, { 0x8d, 0xd6, 0x39, 0xab, 0x4a, 0x9c, 0x19, 0x97 } })

#define XAML_CANVAS_VTBL(type)                  \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type)); \
    XAML_EVENT(redraw, type)

XAML_DECL_INTERFACE_(xaml_canvas, xaml_control)
{
    XAML_DECL_VTBL(xaml_canvas, XAML_CANVAS_VTBL);
};

#endif // !XAML_UI_CANVAS_CANVAS_H
