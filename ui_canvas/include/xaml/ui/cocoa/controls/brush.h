#ifndef XAML_UI_CANVAS_COCOA_BRUSH_H
#define XAML_UI_CANVAS_COCOA_BRUSH_H

#include <xaml/ui/cocoa/objc.h>
#include <xaml/ui/controls/brush.h>

XAML_CLASS(xaml_cocoa_brush, { 0xfdd93db9, 0x0203, 0x454f, { 0x9b, 0x08, 0xc9, 0x62, 0xbc, 0xd4, 0x40, 0x0f } })

#define XAML_COCOA_BRUSH_VTBL(type)            \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(set, type, OBJC_OBJECT(NSBezierPath))

XAML_DECL_INTERFACE_(xaml_cocoa_brush, xaml_object)
{
    XAML_DECL_VTBL(xaml_cocoa_brush, XAML_COCOA_BRUSH_VTBL);
};

#endif // !XAML_UI_CANVAS_COCOA_BRUSH_H
