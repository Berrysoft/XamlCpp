#ifndef XAML_ASYNC_INFO_H
#define XAML_ASYNC_INFO_H

#include <xaml/object.h>

typedef enum xaml_async_status
{
    xaml_async_started,
    xaml_async_completed,
    xaml_async_error
} xaml_async_status;

XAML_TYPE(xaml_async_status, { 0x2e1ff26a, 0x5de9, 0x4d44, { 0x8d, 0xbd, 0xcf, 0xf6, 0xd2, 0xe8, 0x12, 0xf7 } })

XAML_CLASS(xaml_async_info, { 0x0d9e61f6, 0x5eb4, 0x4d82, { 0x9e, 0xb0, 0x98, 0x2f, 0xf8, 0x20, 0x82, 0x23 } })

#define XAML_ASYNC_INFO_VTBL(type)             \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(get_error, type);              \
    XAML_METHOD(get_status, type, xaml_async_status*)

XAML_DECL_INTERFACE_(xaml_async_info, xaml_object)
{
    XAML_DECL_VTBL(xaml_async_info, XAML_ASYNC_INFO_VTBL);
};

#endif // !XAML_ASYNC_INFO_H
