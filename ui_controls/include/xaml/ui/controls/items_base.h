#ifndef XAML_UI_CONTROLS_ITEMS_BASE_H
#define XAML_UI_CONTROLS_ITEMS_BASE_H

#include <xaml/observable_vector.h>
#include <xaml/ui/control.h>

XAML_CLASS(xaml_items_base, { 0xfad67a33, 0x4a5f, 0x4915, { 0xa1, 0x77, 0xe6, 0x1e, 0x89, 0xc7, 0x5e, 0x26 } })

#define XAML_ITEMS_BASE_VTBL(type)                                             \
    XAML_VTBL_INHERIT(XAML_CONTROL_VTBL(type));                                \
    XAML_PROP(items, type, xaml_observable_vector**, xaml_observable_vector*); \
    XAML_EVENT(items_changed, type);                                           \
    XAML_PROP(sel_id, type, XAML_CSTD int32_t*, XAML_CSTD int32_t);            \
    XAML_EVENT(sel_id_changed, type)

XAML_DECL_INTERFACE_(xaml_items_base, xaml_control)
{
    XAML_DECL_VTBL(xaml_items_base, XAML_ITEMS_BASE_VTBL);
};

#endif // !XAML_UI_CONTROLS_ITEMS_BASE_H
