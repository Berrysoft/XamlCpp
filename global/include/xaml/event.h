#ifndef XAML_EVENT_H
#define XAML_EVENT_H

#include <xaml/callback.h>
#include <xaml/object.h>
#include <xaml/vector.h>

XAML_CLASS(xaml_event, { 0x84577f0b, 0xaf47, 0x4f60, { 0x8e, 0xe6, 0x69, 0x6b, 0x2b, 0xdd, 0xca, 0x7f } })

#ifdef __cplusplus
struct xaml_event : xaml_callback
{
    virtual xaml_result XAML_CALL add(xaml_callback*, size_t*) noexcept = 0;
    virtual xaml_result XAML_CALL remove(size_t) noexcept = 0;
};
#else
#define XAML_EVENT_VTBL(type)                                        \
    xaml_result(XAML_CALL* add)(type* const, xaml_callback*, size*); \
    xaml_result(XAML_CALL* remove)(type* const, size_t);

struct xaml_event
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_event)
        XAML_EVENT_VTBL(xaml_event)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_event_new(xaml_event**) XAML_NOEXCEPT;

#endif // !XAML_EVENT_H