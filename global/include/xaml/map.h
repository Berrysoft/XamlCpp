#ifndef XAML_MAP_H
#define XAML_MAP_H

#ifdef __cplusplus
#include <unordered_map>
#include <xaml/xaml_ptr.hpp>
#endif // __cplusplus

#include <xaml/enumerable.h>

XAML_CLASS(xaml_key_value_pair, { 0x41ea97bb, 0xda95, 0x430c, { 0xa8, 0x1b, 0xdb, 0x06, 0x61, 0x9f, 0x7e, 0x96 } })

#ifdef __cplusplus
struct xaml_key_value_pair : xaml_object
{
    virtual xaml_result XAML_CALL get_key(xaml_object**) noexcept = 0;
    virtual xaml_result XAML_CALL get_value(xaml_object**) noexcept = 0;
};
#else
#define XAML_KEY_VALUE_PAIR_VTBL(type)                           \
    xaml_result(XAML_CALL* get_key)(type* const, xaml_object**); \
    xaml_result(XAML_CALL* get_value)(type* const, xaml_object**);

struct xaml_key_value_pair
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_key_value_pair)
        XAML_KEY_VALUE_PAIR_VTBL(xaml_key_value_pair)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_key_value_pair_new(xaml_object*, xaml_object*, xaml_key_value_pair**) XAML_NOEXCEPT;

XAML_CLASS(xaml_map_view, { 0x15549c22, 0x40d1, 0x4af1, { 0xad, 0x81, 0x0d, 0xd3, 0xda, 0x1a, 0x87, 0xba } })

#ifdef __cplusplus
struct xaml_map_view : xaml_enumerable
{
    virtual xaml_result XAML_CALL lookup(xaml_object*, xaml_object**) noexcept = 0;
    virtual xaml_result XAML_CALL has_key(xaml_object*, bool*) noexcept = 0;
    virtual xaml_result XAML_CALL get_size(std::size_t*) noexcept = 0;
};
#else
#define XAML_MAP_VIEW_VTBL(type)                                              \
    xaml_result(XAML_CALL* lookup)(type* const, xaml_object*, xaml_object**); \
    xaml_result(XAML_CALL* has_key)(type* const, xaml_object*, _Bool*);       \
    xaml_result(XAML_CALL* get_size)(type* const, size_t*);

struct xaml_map_view
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_map_view)
        XAML_ENUMERABLE_VTBL(xaml_map_view)
        XAML_MAP_VIEW_VTBL(xaml_map_view)
    } const* vtbl;
};
#endif // __cplusplus

XAML_CLASS(xaml_map, { 0xe457ba53, 0x03d1, 0x4047, { 0xac, 0x85, 0x83, 0xdd, 0x85, 0x34, 0x6c, 0x7e } })

#ifdef __cplusplus
struct xaml_map : xaml_map_view
{
    virtual xaml_result XAML_CALL insert(xaml_object*, xaml_object*, bool*) noexcept = 0;
    virtual xaml_result XAML_CALL remove(xaml_object*) noexcept = 0;
    virtual xaml_result XAML_CALL clear() noexcept = 0;
};
#else
#define XAML_MAP_VTBL(type)                                                          \
    xaml_result(XAML_CALL* insert)(type* const, xaml_object*, xaml_object*, _Bool*); \
    xaml_result(XAML_CALL* remove)(type* const, xaml_object*);                       \
    xaml_result(XAML_CALL* clear)(type * cosnt);

struct xaml_map
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_map)
        XAML_ENUMERABLE_VTBL(xaml_map)
        XAML_MAP_VIEW_VTBL(xaml_map)
        XAML_MAP_VTBL(xaml_map)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_map_new(xaml_map**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_API xaml_result xaml_map_new(std::unordered_map<xaml_ptr<xaml_object>, xaml_ptr<xaml_object>>&&, xaml_map**) noexcept;
#endif // __cplusplus

#endif // !XAML_MAP_H
