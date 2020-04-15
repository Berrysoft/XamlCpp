#ifndef XAML_ENUMERABLE_HPP
#define XAML_ENUMERABLE_HPP

#include <stdbool.h>
#include <xaml/object.h>

typedef struct xaml_enumerator xaml_enumerator;

#ifdef __cplusplus
struct xaml_enumerator : xaml_object
{
    virtual xaml_result XAML_CALL move_next(bool*) = 0;
    virtual xaml_result XAML_CALL get_current(xaml_object**) = 0;
};
#else
struct xaml_enumerator
{
    struct
    {
        size_t(XAML_CALL* add_ref)(xaml_enumerator*);
        size_t(XAML_CALL* release)(xaml_enumerator*);
        xaml_result(XAML_CALL* query)(xaml_enumerator*, xaml_guid XAML_CONST_REF, xaml_object**);
        xaml_result(XAML_CALL* move_next)(xaml_enumerator*, bool*);
        xaml_result(XAML_CALL* get_current)(xaml_enumerator*, xaml_object**);
    } const* vtbl;
};
#endif // __cplusplus

typedef struct xaml_enumerable xaml_enumerable;

#ifdef __cplusplus
struct xaml_enumerable : xaml_object
{
    virtual xaml_result XAML_CALL get_enumerator(xaml_enumerator**) = 0;
};
#else
struct xaml_enumerable
{
    struct
    {
        size_t(XAML_CALL* add_ref)(xaml_enumerable*);
        size_t(XAML_CALL* release)(xaml_enumerable*);
        xaml_result(XAML_CALL* query)(xaml_enumerable*, xaml_guid XAML_CONST_REF, xaml_object**);
        xaml_result(XAML_CALL* get_enumerator)(xaml_enumerable*, xaml_enumerator**);
    } const* vtbl;
};
#endif // __cplusplus

#endif // !XAML_ENUMERABLE_HPP
