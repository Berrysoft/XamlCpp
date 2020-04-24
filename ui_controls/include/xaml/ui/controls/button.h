#ifndef XAML_UI_CONTROLS_BUTTON_H
#define XAML_UI_CONTROLS_BUTTON_H

#include <xaml/ui/control.h>

XAML_CLASS(xaml_button, { 0xe35d0bbe, 0xbec8, 0x4376, { 0xaa, 0xb6, 0xa9, 0x01, 0xed, 0xb4, 0x3d, 0x9f } })

#define XAML_BUTTON_VTBL(type)                          \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));         \
    XAML_PROP(text, type, xaml_string**, xaml_string*); \
    XAML_EVENT(text_changed, type);                     \
    XAML_PROP(is_default, type, bool*, bool);           \
    XAML_EVENT(is_default_changed, type);               \
    XAML_EVENT(click)

XAML_DECL_INTERFACE_(xaml_button, xaml_control)
{
    XAML_DECL_VTBL(xaml_button, XAML_BUTTON_VTBL);
};

#endif // !XAML_UI_CONTROLS_BUTTON_H
