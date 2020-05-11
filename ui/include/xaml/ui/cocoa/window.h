#ifndef XAML_UI_COCOA_WINDOW_H
#define XAML_UI_COCOA_WINDOW_H

#include <xaml/ui/cocoa/control.h>

// TODO: change guid
XAML_CLASS(xaml_cocoa_window, { 0xd062e63e, 0x6c5a, 0x4543, { 0xa7, 0x47, 0x1f, 0x31, 0x19, 0x7d, 0xfb, 0x82 } })

#define XAML_COCOA_WINDOW_VTBL(type)                                        \
    XAML_VTBL_INHERIT(XAML_COCOA_CONTROL_VTBL(type));                       \
    XAML_PROP(window, type, OBJC_OBJECT(NSWindow)*, OBJC_OBJECT(NSWindow)); \
    XAML_PROP(menu_bar, type, OBJC_OBJECT(NSMenu)*, OBJC_OBJECT(NSMenu))

XAML_DECL_INTERFACE_(xaml_cocoa_window, xaml_cocoa_control)
{
    XAML_DECL_VTBL(xaml_cocoa_window, XAML_COCOA_WINDOW_VTBL);
};

#endif // !XAML_UI_COCOA_WINDOW_H
