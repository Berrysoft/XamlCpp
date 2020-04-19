#ifndef XAML_META_MODULE_H
#define XAML_META_MODULE_H

#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_module, { 0x03e2b52a, 0x7f9d, 0x4cc8, { 0x84, 0x70, 0x30, 0x0e, 0x3e, 0x6d, 0x5e, 0x5f } })

#ifdef __cplusplus
struct XAML_NOVTBL xaml_module : xaml_object
{
    virtual xaml_result XAML_CALL open(xaml_string*) noexcept = 0;
    virtual xaml_result XAML_CALL get_method(char const*, void**) noexcept = 0;
};
#else
#define XAML_MODULE_VTBL(type)                               \
    XAML_OBJECT_VTBL(type)                                   \
    xaml_result(XAML_CALL* open)(type* const, xaml_string*); \
    xaml_result(XAML_CALL* get_method)(type* const, char const*, void**);

struct xaml_module
{
    struct
    {
        XAML_MODULE_VTBL(xaml_module)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_META_API xaml_result xaml_module_new(xaml_module**) XAML_NOEXCEPT;

#endif // !XAML_META_MODULE_H
