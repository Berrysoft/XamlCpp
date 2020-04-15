#ifndef XAML_VECTOR_H
#define XAML_VECTOR_H

#include <xaml/enumerable.h>

typedef struct xaml_vector_view xaml_vector_view;

#ifdef __cplusplus
struct xaml_vector_view : xaml_enumerable
{
    virtual xaml_result XAML_CALL get_at(size_t, xaml_object**) = 0;
};
#else
struct xaml_vector_view
{
    struct
    {
        size_t(XAML_CALL* add_ref)(xaml_vector_view*);
        size_t(XAML_CALL* release)(xaml_vector_view*);
        xaml_result(XAML_CALL* query)(xaml_vector_view*, xaml_guid XAML_CONST_REF, xaml_object**);
        xaml_result(XAML_CALL* get_enumerator)(xaml_vector_view*, xaml_enumerator**);
        xaml_result(XAML_CALL* get_at)(xaml_vector_view*, size_t, xaml_object**);
    } const* vtbl;
};
#endif // __cplusplus

typedef struct xaml_vector xaml_vector;

#ifdef __cplusplus
struct xaml_vector : xaml_vector_view
{
    virtual xaml_result XAML_CALL set_at(size_t, xaml_object*) = 0;
    virtual xaml_result XAML_CALL append(xaml_object*) = 0;
    virtual xaml_result XAML_CALL remove_at(size_t) = 0;
};
#else
struct xaml_vector
{
    struct
    {
        size_t(XAML_CALL* add_ref)(xaml_vector*);
        size_t(XAML_CALL* release)(xaml_vector*);
        xaml_result(XAML_CALL* query)(xaml_vector*, xaml_guid XAML_CONST_REF, xaml_object**);
        xaml_result(XAML_CALL* get_enumerator)(xaml_vector*, xaml_enumerator**);
        xaml_result(XAML_CALL* get_at)(xaml_vector*, size_t, xaml_object**);
        xaml_result(XAML_CALL* set_at)(xaml_vector*, size_t, xaml_object*);
        xaml_result(XAML_CALL* append)(xaml_vector*, xaml_object*);
        xaml_result(XAML_CALL* remove_at)(xaml_vector*, size_t);
    } const* vtbl;
};
#endif // __cplusplus

#endif // !XAML_VECTOR_H
