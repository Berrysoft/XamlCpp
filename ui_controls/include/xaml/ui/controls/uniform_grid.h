#ifndef XAML_UI_CONTROLS_UNIFORM_GRID_H
#define XAML_UI_CONTROLS_UNIFORM_GRID_H

#include <xaml/ui/controls/layout_base.h>

XAML_CLASS(xaml_uniform_grid, { 0x1963c67d, 0xdc81, 0x40dd, { 0x91, 0x2e, 0xa6, 0xa9, 0x44, 0x48, 0xf2, 0x37 } })

#define XAML_UNIFORM_GRID_VTBL(type)                                 \
    XAML_VTBL_INHERIT(XAML_LAYOUT_BASE_VTBL(type));                  \
    XAML_PROP(columns, type, XAML_CSTD int32_t*, XAML_CSTD int32_t); \
    XAML_PROP(rows, type, XAML_CSTD int32_t*, XAML_CSTD int32_t);    \
    XAML_PROP(orientation, type, xaml_orientation*, xaml_orientation)

XAML_DECL_INTERFACE_(xaml_uniform_grid, xaml_layout_base)
{
    XAML_DECL_VTBL(xaml_uniform_grid, XAML_UNIFORM_GRID_VTBL);
};

#endif // !XAML_UI_CONTROLS_UNIFORM_GRID_H
