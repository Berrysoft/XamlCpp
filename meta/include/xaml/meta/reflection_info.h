#ifndef XAML_META_REFLECTION_INFO_H
#define XAML_META_REFLECTION_INFO_H

#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_reflection_info, { 0x5abbe92c, 0x833d, 0x4d24, { 0xa7, 0x06, 0x1d, 0x5c, 0xe5, 0x14, 0x1b, 0x0a } })

#ifdef __cplusplus
struct xaml_reflection_info : xaml_object
{
    virtual xaml_result XAML_CALL get_type(xaml_guid*) noexcept = 0;
    virtual xaml_result XAML_CALL get_type_name(xaml_string**) noexcept = 0;
    virtual xaml_result XAML_CALL get_include_file(xaml_string**) noexcept = 0;
};
#else
#define XAML_REFLECTION_INFO_VTBL(type)                   \
    xaml_result(XAML_CALL* get_type)(xaml_guid*);         \
    xaml_result(XAML_CALL* get_type_name)(xaml_string**); \
    xaml_result(XAML_CALL* get_include_file)(xaml_string**);

struct xaml_reflection_info
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_reflection_info)
        XAML_REFLECTION_INFO_VTBL(xaml_reflection_info)
    } const* vtbl;
};
#endif // __cplusplus

#endif // !XAML_META_REFLECTION_INFO_H
