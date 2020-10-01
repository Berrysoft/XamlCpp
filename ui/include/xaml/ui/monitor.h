#ifndef XAML_UI_SCREEN_H
#define XAML_UI_SCREEN_H

#ifdef __cplusplus
    #include <compare>
#endif // __cplusplus

#include <xaml/ui/drawing.h>
#include <xaml/utility.h>
#include <xaml/vector.h>

typedef struct xaml_monitor xaml_monitor;

struct xaml_monitor
{
    xaml_rectangle region;
    xaml_rectangle client;

#ifdef __cplusplus
    auto operator<=>(xaml_monitor const&) const = default;
#endif // __cplusplus
};

XAML_TYPE(xaml_monitor, { 0xc81625a5, 0xb58f, 0x403e, { 0x92, 0xbb, 0x1a, 0xe2, 0xdf, 0x12, 0x4d, 0xca } })

#ifndef xaml_enumerator_1__xaml_monitor_defined
    #define xaml_enumerator_1__xaml_monitor_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_V(xaml_monitor))
#endif // !xaml_enumerator_1__xaml_monitor_defined

#ifndef xaml_vector_view_1__xaml_monitor_defined
    #define xaml_vector_view_1__xaml_monitor_defined
XAML_VECTOR_VIEW_1_TYPE(XAML_T_V(xaml_monitor))
#endif // !xaml_vector_view_1__xaml_monitor_defined

EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_monitor_get_all(XAML_VECTOR_VIEW_1_NAME(xaml_monitor) **) XAML_NOEXCEPT;

#endif // !XAML_UI_SCREEN_H
