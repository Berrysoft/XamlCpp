#ifndef XAML_UI_COCOA_CONTROL_H
#define XAML_UI_COCOA_CONTROL_H

#include <xaml/ui/cocoa/objc.h>
#include <xaml/ui/control.h>

#ifdef __OBJC__
    #import <xaml/ui/cocoa/XamlDelegate.h>
#endif // __OBJC__

XAML_CLASS(xaml_cocoa_control, { 0x5ecf49d1, 0x2729, 0x4eca, { 0x9e, 0xc7, 0xc3, 0x7d, 0xc8, 0xef, 0xb8, 0x6b } })

#define XAML_COCOA_CONTROL_VTBL(type)                                   \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                          \
    XAML_PROP(handle, type, OBJC_OBJECT(NSView)*, OBJC_OBJECT(NSView)); \
    XAML_PROP(delegate, type, OBJC_OBJECT(XamlDelegate)*, OBJC_OBJECT(XamlDelegate))

XAML_DECL_INTERFACE_(xaml_cocoa_control, xaml_object)
{
    XAML_DECL_VTBL(xaml_cocoa_control, XAML_COCOA_CONTROL_VTBL);
};

#endif // !XAML_UI_COCOA_CONTROL_H