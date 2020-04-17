#ifndef XAML_META_EVENT_INFO_H
#define XAML_META_EVENT_INFO_H

#include <xaml/delegate.h>
#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_event_info, { 0x3e92f836, 0x4337, 0x40c7, { 0x83, 0x99, 0x52, 0x7e, 0x46, 0xd4, 0x30, 0x48 } })

#ifdef __cplusplus
struct xaml_event_info : xaml_object
{
    virtual xaml_result XAML_CALL get_name(xaml_string**) const noexcept = 0;
    virtual xaml_result XAML_CALL add(xaml_object*, xaml_delegate*, std::size_t*) const noexcept = 0;
    virtual xaml_result XAML_CALL remove(xaml_object*, std::size_t) const noexcept = 0;
};
#else

#endif // __cplusplus

#endif // !XAML_META_EVENT_INFO_H
