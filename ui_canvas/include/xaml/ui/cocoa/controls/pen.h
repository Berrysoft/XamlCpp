#ifndef XAML_UI_CANVAS_COCOA_PEN_H
#define XAML_UI_CANVAS_COCOA_PEN_H

#include <xaml/ui/cocoa/objc.h>
#include <xaml/ui/controls/pen.h>

XAML_CLASS(xaml_cocoa_pen, { 0xd34daff9, 0x2b11, 0x4557, { 0xbc, 0x6b, 0xef, 0xf3, 0x90, 0x15, 0xe9, 0xbb } })

#define XAML_COCOA_PEN_VTBL(type)              \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(set, type, OBJC_OBJECT(NSBezierPath))

XAML_DECL_INTERFACE_(xaml_cocoa_pen, xaml_object)
{
    XAML_DECL_VTBL(xaml_cocoa_pen, XAML_COCOA_PEN_VTBL);
};

#endif // !XAML_UI_CANVAS_COCOA_PEN_H
