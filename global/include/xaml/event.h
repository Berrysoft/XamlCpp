#ifndef XAML_EVENT_H
#define XAML_EVENT_H

#include <xaml/delegate.h>
#include <xaml/object.h>

XAML_CLASS(xaml_event, { 0x84577f0b, 0xaf47, 0x4f60, { 0x8e, 0xe6, 0x69, 0x6b, 0x2b, 0xdd, 0xca, 0x7f } })

#define XAML_EVENT_VTBL(type)                                  \
    XAML_VTBL_INHERIT(XAML_DELEGATE_VTBL(type));               \
    XAML_METHOD(add, type, xaml_delegate*, XAML_CSTD size_t*); \
    XAML_METHOD(remove, type, XAML_CSTD size_t)

XAML_DECL_INTERFACE_(xaml_event, xaml_delegate)
{
    XAML_DECL_VTBL(xaml_event, XAML_EVENT_VTBL);
};

EXTERN_C XAML_API xaml_result xaml_event_new(xaml_event**) XAML_NOEXCEPT;

#endif // !XAML_EVENT_H
