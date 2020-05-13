#ifndef XAML_UI_COCOA_WINDOW_H
#define XAML_UI_COCOA_WINDOW_H

#include <xaml/ui/cocoa/control.h>

XAML_CLASS(xaml_cocoa_window, { 0x6f0d30d7, 0x3c5d, 0x4868, { 0xbb, 0xce, 0x6d, 0x4f, 0x0b, 0x75, 0xfa, 0xda } })

#define XAML_COCOA_WINDOW_VTBL(type)                                        \
    XAML_VTBL_INHERIT(XAML_COCOA_CONTROL_VTBL(type));                       \
    XAML_PROP(window, type, OBJC_OBJECT(NSWindow)*, OBJC_OBJECT(NSWindow)); \
    XAML_PROP(menu_bar, type, OBJC_OBJECT(NSMenu)*, OBJC_OBJECT(NSMenu))

XAML_DECL_INTERFACE_(xaml_cocoa_window, xaml_cocoa_control)
{
    XAML_DECL_VTBL(xaml_cocoa_window, XAML_COCOA_WINDOW_VTBL);
};

#endif // !XAML_UI_COCOA_WINDOW_H
