#ifndef XAML_UI_CANVAS_WIN_BRUSH_D2D_H
#define XAML_UI_CANVAS_WIN_BRUSH_D2D_H

#include <d2d1.h>
#include <xaml/ui/controls/brush.h>

XAML_CLASS(xaml_win32_brush, { 0xf471fc33, 0x03af, 0x4959, { 0xa9, 0x52, 0xcc, 0x92, 0xea, 0xeb, 0xdd, 0xb1 } })

#define XAML_WIN32_BRUSH_VTBL(type)            \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(create, type, ID2D1RenderTarget*, ID2D1Brush**)

XAML_DECL_INTERFACE_(xaml_win32_brush, xaml_object)
{
    XAML_DECL_VTBL(xaml_win32_brush, XAML_WIN32_BRUSH_VTBL);
};

#endif // !XAML_UI_CANVAS_WIN_BRUSH_D2D_H
