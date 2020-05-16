#ifndef XAML_BUFFER_H
#define XAML_BUFFER_H

#ifdef __cplusplus
#include <vector>
#endif // __cplusplus

#include <xaml/object.h>

XAML_CLASS(xaml_buffer, { 0xcfd8027c, 0x82e7, 0x4d77, { 0xa5, 0xb4, 0x06, 0xca, 0x8d, 0xea, 0xb7, 0xec } })

#define XAML_BUFFER_VTBL(type)                      \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));      \
    XAML_METHOD(get_size, type, XAML_STD int32_t*); \
    XAML_METHOD(get_data, type, XAML_STD uint8_t**)

XAML_DECL_INTERFACE_(xaml_buffer, xaml_object)
{
    XAML_DECL_VTBL(xaml_buffer, XAML_BUFFER_VTBL);
};

EXTERN_C XAML_API xaml_result XAML_CALL xaml_buffer_new(XAML_STD int32_t, xaml_buffer**) XAML_NOEXCEPT;
EXTERN_C XAML_API xaml_result XAML_CALL xaml_buffer_new_reference(XAML_STD uint8_t*, XAML_STD int32_t, xaml_buffer**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result XAML_CALL xaml_buffer_new(std::vector<std::uint8_t>&&, xaml_buffer**) noexcept;
XAML_API xaml_result XAML_CALL xaml_buffer_new_reference(std::vector<std::uint8_t>&, xaml_buffer**) noexcept;
#endif // __cplusplus

#endif // !XAML_BUFFER_H
