#ifndef XAML_META_ENUM_INFO_H
#define XAML_META_ENUM_INFO_H

#ifdef __cplusplus
#include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/box.h>
#include <xaml/map.h>
#include <xaml/meta/reflection_info.h>

XAML_CLASS(xaml_enum_info, { 0x51dcc841, 0xc0d0, 0x4c8b, { 0x9f, 0x9f, 0xed, 0x7b, 0x48, 0xa1, 0xa9, 0xd9 } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_enum_info : xaml_reflection_info
{
    virtual xaml_result XAML_CALL get_values(xaml_map_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_value(xaml_string*, xaml_box**) noexcept = 0;

    template <typename T>
    xaml_result get_value(xaml_string* str, T& value)
    {
        xaml_ptr<xaml_box> box;
        XAML_RETURN_IF_FAILED(get_value(str, &box));
        return unbox_value(box.get(), value);
    }
};
#else
#define XAML_ENUM_INFO_VTBL(type)                        \
    XAML_REFLECTION_INFO_VTBL(type)                      \
    xaml_result(XAML_CALL* get_values)(xaml_map_view**); \
    xaml_result(XAML_CALL* get_value)(xaml_string*, xaml_box**);

struct xaml_enum_info
{
    struct
    {
        XAML_ENUM_INFO_VTBL(xaml_enum_info)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_META_API xaml_result xaml_enum_info_new(xaml_map_view*, xaml_enum_info**) XAML_NOEXCEPT;

#endif // !XAML_META_ENUM_INFO_H
