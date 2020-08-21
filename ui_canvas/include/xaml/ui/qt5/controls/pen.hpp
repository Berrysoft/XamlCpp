#ifndef XAML_UI_CANVAS_QT5_PEN_HPP
#define XAML_UI_CANVAS_QT5_PEN_HPP

#include <QPen>
#include <xaml/ui/controls/brush.h>

XAML_CLASS(xaml_qt5_pen, { 0x636e3b1a, 0x0415, 0x49fc, { 0xaf, 0x86, 0xd0, 0x81, 0x71, 0x6f, 0xb6, 0x37 } })

#define XAML_QT5_PEN_VTBL(type)                \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(create, type, xaml_rectangle XAML_CONST_REF, QPen*)

XAML_DECL_INTERFACE_(xaml_qt5_pen, xaml_object)
{
    XAML_DECL_VTBL(xaml_qt5_pen, XAML_QT5_PEN_VTBL);
};

#endif // !XAML_UI_CANVAS_QT5_PEN_HPP
