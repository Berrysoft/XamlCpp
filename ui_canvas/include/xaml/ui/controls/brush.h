#ifndef XAML_UI_CANVAS_BRUSH_H
#define XAML_UI_CANVAS_BRUSH_H

#ifdef __cplusplus
    #include <compare>
#endif // __cplusplus

#include <xaml/meta/meta_macros.h>
#include <xaml/ui/drawing.h>

XAML_CLASS(xaml_brush, { 0x6b543544, 0x961a, 0x4096, { 0xb3, 0x7b, 0x2c, 0x79, 0xaa, 0xbc, 0xd4, 0x39 } })

#define XAML_BRUSH_VTBL(type) XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type))

XAML_DECL_INTERFACE_(xaml_brush, xaml_object)
{
    XAML_DECL_VTBL(xaml_brush, XAML_BRUSH_VTBL);
};

XAML_CLASS(xaml_solid_brush, { 0x4a1295f2, 0xe5b9, 0x4bb2, { 0xa5, 0x90, 0x78, 0x59, 0xaf, 0x59, 0x8d, 0x9e } })

#define XAML_SOLID_BRUSH_VTBL(type)           \
    XAML_VTBL_INHERIT(XAML_BRUSH_VTBL(type)); \
    XAML_PROP(color, type, xaml_color*, xaml_color)

XAML_DECL_INTERFACE_(xaml_solid_brush, xaml_brush)
{
    XAML_DECL_VTBL(xaml_solid_brush, XAML_SOLID_BRUSH_VTBL);
};

EXTERN_C XAML_UI_CANVAS_API xaml_result XAML_CALL xaml_solid_brush_new(xaml_color, xaml_solid_brush**) XAML_NOEXCEPT;

typedef struct xaml_gradient_stop
{
    xaml_color color;
    double position;

#ifdef __cplusplus
    auto operator<=>(xaml_gradient_stop const&) const = default;
#endif // __cplusplus
} xaml_gradient_stop;

XAML_TYPE(xaml_gradient_stop, { 0xe2ccbf44, 0x1909, 0x41c6, { 0x88, 0x85, 0x70, 0x7c, 0x94, 0x2e, 0xe4, 0xe5 } })

#ifndef xaml_enumerator_1__xaml_gradient_stop_defined
    #define xaml_enumerator_1__xaml_gradient_stop_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_V(xaml_gradient_stop))
#endif // !xaml_enumerator_1__xaml_gradient_stop_defined

#ifndef xaml_vector_view_1__xaml_gradient_stop_defined
    #define xaml_vector_view_1__xaml_gradient_stop_defined
XAML_VECTOR_VIEW_1_TYPE(XAML_T_V(xaml_gradient_stop))
#endif // !xaml_vector_view_1__xaml_gradient_stop_defined

XAML_CLASS(xaml_gradient_brush, { 0x6fca23d8, 0xc758, 0x484c, { 0x91, 0x2b, 0xc0, 0x94, 0x03, 0x59, 0xa4, 0x77 } })

#define XAML_GRADIENT_BRUSH_VTBL(type)                                           \
    XAML_VTBL_INHERIT(XAML_BRUSH_VTBL(type));                                    \
    XAML_METHOD(get_stops, type, XAML_VECTOR_VIEW_1_NAME(xaml_gradient_stop)**); \
    XAML_CPROP(stop, type, xaml_gradient_stop XAML_CONST_REF, xaml_gradient_stop XAML_CONST_REF)

XAML_DECL_INTERFACE_(xaml_gradient_brush, xaml_brush)
{
    XAML_DECL_VTBL(xaml_gradient_brush, XAML_GRADIENT_BRUSH_VTBL);
};

XAML_CLASS(xaml_linear_gradient_brush, { 0xfb86a888, 0x0a5f, 0x47a9, { 0x81, 0x8c, 0xa9, 0x48, 0xbd, 0xe2, 0x9e, 0x07 } })

#define XAML_LINEAR_GRADIENT_BRUSH_VTBL(type)                             \
    XAML_VTBL_INHERIT(XAML_GRADIENT_BRUSH_VTBL(type));                    \
    XAML_PROP(start_point, type, xaml_point*, xaml_point XAML_CONST_REF); \
    XAML_PROP(end_point, type, xaml_point*, xaml_point XAML_CONST_REF)

XAML_DECL_INTERFACE_(xaml_linear_gradient_brush, xaml_gradient_brush)
{
    XAML_DECL_VTBL(xaml_linear_gradient_brush, XAML_LINEAR_GRADIENT_BRUSH_VTBL);
};

EXTERN_C XAML_UI_CANVAS_API xaml_result XAML_CALL xaml_linear_gradient_brush_new(xaml_linear_gradient_brush**) XAML_NOEXCEPT;

XAML_CLASS(xaml_radial_gradient_brush, { 0xeb863aa0, 0x2d59, 0x4128, { 0x80, 0xe6, 0x65, 0x71, 0x8d, 0x83, 0x18, 0x8b } })

#define XAML_RADIAL_GRADIENT_BRUSH_VTBL(type)                        \
    XAML_VTBL_INHERIT(XAML_GRADIENT_BRUSH_VTBL(type));               \
    XAML_PROP(origin, type, xaml_point*, xaml_point XAML_CONST_REF); \
    XAML_PROP(center, type, xaml_point*, xaml_point XAML_CONST_REF); \
    XAML_PROP(radius, type, xaml_size*, xaml_size XAML_CONST_REF)

XAML_DECL_INTERFACE_(xaml_radial_gradient_brush, xaml_gradient_brush)
{
    XAML_DECL_VTBL(xaml_radial_gradient_brush, XAML_RADIAL_GRADIENT_BRUSH_VTBL);
};

EXTERN_C XAML_UI_CANVAS_API xaml_result XAML_CALL xaml_radial_gradient_brush_new(xaml_radial_gradient_brush**) XAML_NOEXCEPT;

#endif // !XAML_UI_CANVAS_BRUSH_H
