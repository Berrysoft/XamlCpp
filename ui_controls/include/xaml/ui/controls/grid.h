#ifndef XAML_UI_CONTROLS_GRID_H
#define XAML_UI_CONTROLS_GRID_H

#include <xaml/ui/controls/layout_base.h>

typedef enum xaml_grid_layout
{
    xaml_grid_layout_abs,
    xaml_grid_layout_star,
    xaml_grid_layout_auto
} xaml_grid_layout;

XAML_TYPE(xaml_grid_layout, { 0x61521be2, 0x02fd, 0x40b8, { 0xb2, 0xa2, 0x5f, 0x60, 0xd9, 0x6b, 0x5c, 0x39 } })

typedef struct xaml_grid_length xaml_grid_length;

struct xaml_grid_length
{
    double value;
    xaml_grid_layout layout;
};

XAML_TYPE(xaml_grid_length, { 0xfed37e9d, 0x1f3b, 0x4247, { 0x82, 0x69, 0x95, 0x56, 0x09, 0x93, 0xf3, 0x84 } })

XAML_CLASS(xaml_grid, { 0x72737dd1, 0x7c10, 0x46e6, { 0x82, 0x38, 0x06, 0x2f, 0x43, 0x6b, 0xb0, 0x24 } })

#define XAML_GRID_VTBL(type)                                                      \
    XAML_VTBL_INHERIT(XAML_LAYOUT_BASE_VTBL(type));                               \
    XAML_METHOD(get_columns, type, xaml_vector_view**);                           \
    XAML_METHOD(get_rows, type, xaml_vector_view**);                              \
    XAML_CPROP(column, type, xaml_grid_length*, xaml_grid_length XAML_CONST_REF); \
    XAML_CPROP(row, type, xaml_grid_length*, xaml_grid_length XAML_CONST_REF)

XAML_DECL_INTERFACE_(xaml_grid, xaml_layout_base)
{
    XAML_DECL_VTBL(xaml_grid, XAML_GRID_VTBL);
};

#endif // !XAML_UI_CONTROLS_GRID_H
