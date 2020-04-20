#ifndef XAML_UI_CONTROL_H
#define XAML_UI_CONTROL_H

#include <xaml/delegate.h>
#include <xaml/meta/meta_macros.h>
#include <xaml/object.h>
#include <xaml/ui/drawing.h>

typedef enum xaml_halignment
{
    xaml_halignment_stretch,
    xaml_halignment_left,
    xaml_halignment_center,
    xaml_halignment_right
} xaml_halignment;

typedef enum xaml_valignment
{
    xaml_valignment_stretch,
    xaml_valignment_top,
    xaml_valignment_center,
    xaml_valignment_bottom
} xaml_valignment;

XAML_CLASS(xaml_control, { 0x389f559a, 0x48bb, 0x49a7, { 0xa0, 0x16, 0x1d, 0xcb, 0x95, 0x72, 0x72, 0xa2 } })

#define XAML_CONTROL_VTBL(type)                                        \
    XAML_OBJECT_VTBL(type);                                            \
    XAML_PROP(parent, type, xaml_control**, xaml_control*);            \
    XAML_EVENT(parent_changed, type);                                  \
    XAML_PROP(size, type, xaml_size*, xaml_size XAML_CONST_REF);       \
    XAML_PROP(width, type, double*, double);                           \
    XAML_PROP(height, type, double*, double);                          \
    XAML_EVENT(size_changed, type);                                    \
    XAML_PROP(margin, type, xaml_margin*, xaml_margin XAML_CONST_REF); \
    XAML_EVENT(margin_changed, type);                                  \
    XAML_PROP(halignment, type, xaml_halignment*, xaml_halignment);    \
    XAML_EVENT(halignment_changed, type);                              \
    XAML_PROP(valignment, type, xaml_valignment*, xaml_valignment);    \
    XAML_EVENT(valignment_changed, type);                              \
    XAML_PROP(is_visible, type, bool*, bool);                          \
    XAML_EVENT(is_visible_changed, type)

XAML_DECL_INTERFACE_(xaml_control, xaml_object)
{
    XAML_DECL_VTBL(xaml_control, XAML_CONTROL_VTBL);
};

#endif // !XAML_UI_CONTROL_H
