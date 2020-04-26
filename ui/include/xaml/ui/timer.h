#ifndef XAML_UI_TIMER_H
#define XAML_UI_TIMER_H

#ifdef __cplusplus
#include <chrono>
#else
#include <stdbool.h>
#endif // __cplusplus

#include <xaml/meta/meta_macros.h>
#include <xaml/object.h>

XAML_CLASS(xaml_timer, { 0x21621654, 0xf5ed, 0x4b24, { 0xb2, 0xea, 0x1d, 0xf1, 0x86, 0x92, 0x2d, 0x9c } })

#define XAML_TIMER_VTBL(type)                                       \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                      \
    XAML_PROP(interval, type, XAML_CSTD int32_t*, XAML_CSTD int32_t); \
    XAML_METHOD(get_is_enabled, type, bool*);                       \
    XAML_METHOD(start, type);                                       \
    XAML_METHOD(stop, type);                                        \
    XAML_EVENT(tick, type)

XAML_DECL_INTERFACE_(xaml_timer, xaml_object)
{
    XAML_DECL_VTBL(xaml_timer, XAML_TIMER_VTBL);
};

EXTERN_C XAML_UI_API xaml_result xaml_timer_new(xaml_timer**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result xaml_timer_new_interval(XAML_CSTD int32_t, xaml_timer**) XAML_NOEXCEPT;

#ifdef __cplusplus
inline xaml_result xaml_timer_new_interval(std::chrono::milliseconds ms, xaml_timer** ptr) noexcept
{
    return xaml_timer_new_interval((std::int32_t)ms.count(), ptr);
}
#endif // __cplusplus

#endif // !XAML_UI_TIMER_H
