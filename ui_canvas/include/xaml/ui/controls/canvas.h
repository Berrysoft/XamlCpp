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

XAML_CLASS(xaml_canvas, { 0x111d5785, 0x4e7e, 0x48c6, { 0x8d, 0xd6, 0x39, 0xab, 0x4a, 0x9c, 0x19, 0x97 } })

#endif // !XAML_UI_CANVAS_CANVAS_H
