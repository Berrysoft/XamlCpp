#ifndef XAML_UI_CANVAS_QT5_BRUSH_HPP
#define XAML_UI_CANVAS_QT5_BRUSH_HPP

#include <QBrush>
#include <xaml/ui/controls/brush.h>

XAML_CLASS(xaml_qt5_brush, { 0xa9fb52da, 0xf7f0, 0x4777, { 0xab, 0x86, 0x0d, 0x0d, 0x0f, 0x77, 0xb3, 0xb8 } })

#define XAML_QT5_BRUSH_VTBL(type)              \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(create, type, xaml_rectangle XAML_CONST_REF, QBrush*)

XAML_DECL_INTERFACE_(xaml_qt5_brush, xaml_object)
{
    XAML_DECL_VTBL(xaml_qt5_brush, XAML_QT5_BRUSH_VTBL);
};

#endif // !XAML_UI_CANVAS_QT5_BRUSH_HPP
