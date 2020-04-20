#ifndef XAML_UI_CONTROL_H
#define XAML_UI_CONTROL_H

#include <xaml/delegate.h>
#include <xaml/object.h>

XAML_CLASS(xaml_control, { 0x389f559a, 0x48bb, 0x49a7, { 0xa0, 0x16, 0x1d, 0xcb, 0x95, 0x72, 0x72, 0xa2 } })

#define XAML_CONTROL_VTBL(type)                                               \
    XAML_OBJECT_VTBL(type);                                                   \
    XAML_METHOD(get_parent, type, xaml_control**);                            \
    XAML_METHOD(set_parent, type, xaml_control*);                             \
    XAML_METHOD(add_parent_changed, type, xaml_delegate*, XAML_CSTD size_t*); \
    XAML_METHOD(remove_parent_changed, type, XAML_CSTD size_t)

XAML_DECL_INTERFACE_(xaml_control, xaml_object)
{
    XAML_DECL_VTBL(xaml_control, XAML_CONTROL_VTBL);
};

#endif // !XAML_UI_CONTROL_H
