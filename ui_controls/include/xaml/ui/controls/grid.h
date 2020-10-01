#ifndef XAML_UI_CONTROLS_GRID_H
#define XAML_UI_CONTROLS_GRID_H

#ifdef __cplusplus
    #include <compare>
#endif // __cplusplus

#include <xaml/ui/controls/layout_base.h>

typedef enum xaml_grid_layout
{
    xaml_grid_layout_abs,
    xaml_grid_layout_star,
    xaml_grid_layout_auto
} xaml_grid_layout;

XAML_TYPE(xaml_grid_layout, { 0x61521be2, 0x02fd, 0x40b8, { 0xb2, 0xa2, 0x5f, 0x60, 0xd9, 0x6b, 0x5c, 0x39 } })

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_layout_register(xaml_meta_context*) XAML_NOEXCEPT;

typedef struct xaml_grid_length xaml_grid_length;

struct xaml_grid_length
{
    double value;
    xaml_grid_layout layout;

#ifdef __cplusplus
    auto operator<=>(xaml_grid_length const&) const = default;
#endif // __cplusplus
};

XAML_TYPE(xaml_grid_length, { 0xfed37e9d, 0x1f3b, 0x4247, { 0x82, 0x69, 0x95, 0x56, 0x09, 0x93, 0xf3, 0x84 } })

#ifndef xaml_enumerator_1__xaml_grid_length_defined
    #define xaml_enumerator_1__xaml_grid_length_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_V(xaml_grid_length))
#endif // !xaml_enumerator_1__xaml_grid_length_defined

#ifndef xaml_vector_1__xaml_grid_length_defined
    #define xaml_vector_1__xaml_grid_length_defined
XAML_VECTOR_1_TYPE(XAML_T_V(xaml_grid_length))
#endif // !xaml_vector_1__xaml_grid_length_defined

XAML_CLASS(xaml_grid, { 0x72737dd1, 0x7c10, 0x46e6, { 0x82, 0x38, 0x06, 0x2f, 0x43, 0x6b, 0xb0, 0x24 } })

#define XAML_GRID_VTBL(type)                                                                                 \
    XAML_VTBL_INHERIT(XAML_LAYOUT_BASE_VTBL(type));                                                          \
    XAML_PROP(columns, type, XAML_VECTOR_1_NAME(xaml_grid_length)**, XAML_VECTOR_1_NAME(xaml_grid_length)*); \
    XAML_PROP(rows, type, XAML_VECTOR_1_NAME(xaml_grid_length)**, XAML_VECTOR_1_NAME(xaml_grid_length)*);    \
    XAML_CPROP(column, type, xaml_grid_length XAML_CONST_REF, xaml_grid_length XAML_CONST_REF);              \
    XAML_CPROP(row, type, xaml_grid_length XAML_CONST_REF, xaml_grid_length XAML_CONST_REF)

XAML_DECL_INTERFACE_(xaml_grid, xaml_layout_base)
{
    XAML_DECL_VTBL(xaml_grid, XAML_GRID_VTBL);
};

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_new(xaml_grid**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_register(xaml_meta_context*) XAML_NOEXCEPT;

EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_get_column(xaml_control*, XAML_STD int32_t*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_set_column(xaml_control*, XAML_STD int32_t) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_get_row(xaml_control*, XAML_STD int32_t*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_set_row(xaml_control*, XAML_STD int32_t) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_get_column_span(xaml_control*, XAML_STD int32_t*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_set_column_span(xaml_control*, XAML_STD int32_t) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_get_row_span(xaml_control*, XAML_STD int32_t*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_CONTROLS_API xaml_result XAML_CALL xaml_grid_set_row_span(xaml_control*, XAML_STD int32_t) XAML_NOEXCEPT;

#endif // !XAML_UI_CONTROLS_GRID_H
