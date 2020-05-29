#ifndef XAML_UI_CANVAS_WIN_PEN_H
#define XAML_UI_CANVAS_WIN_PEN_H

#include <d2d1.h>
#include <xaml/ui/controls/pen.h>

XAML_CLASS(xaml_win32_pen, { 0x18d2f0cf, 0xdb54, 0x4fa0, { 0xa4, 0x30, 0xf9, 0x3a, 0x02, 0xe3, 0xe6, 0xff } })

#define XAML_WIN32_PEN_VTBL(type)              \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(create, type, ID2D1RenderTarget*, xaml_rectangle XAML_CONST_REF, ID2D1Brush**, FLOAT*)

XAML_DECL_INTERFACE_(xaml_win32_pen, xaml_object)
{
    XAML_DECL_VTBL(xaml_win32_pen, XAML_WIN32_PEN_VTBL);
};

#endif // !XAML_UI_CANVAS_WIN_PEN_H
