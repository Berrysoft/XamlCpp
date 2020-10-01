#ifndef XAML_META_MODULE_H
#define XAML_META_MODULE_H

#include <xaml/object.h>
#include <xaml/string.h>
#include <xaml/vector.h>
#include <xaml/version.h>

typedef struct xaml_meta_context xaml_meta_context;

#ifndef xaml_enumerator_1__xaml_string_defined
    #define xaml_enumerator_1__xaml_string_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_enumerator_1__xaml_string_defined

#ifndef xaml_vector_view_1__xaml_string_defined
    #define xaml_vector_view_1__xaml_string_defined
XAML_VECTOR_VIEW_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_vector_view_1__xaml_string_defined

XAML_CLASS(xaml_module_info, { 0x29dfda45, 0xb751, 0x42af, { 0x94, 0x43, 0xc1, 0x68, 0xd7, 0x41, 0x80, 0x8b } })

#define XAML_MODULE_INFO_VTBL(type)                                              \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                                   \
    XAML_METHOD(get_version, type, xaml_version*);                               \
    XAML_METHOD(get_dependencies, type, XAML_VECTOR_VIEW_1_NAME(xaml_string)**); \
    XAML_METHOD(register_types, type, xaml_meta_context*)

XAML_DECL_INTERFACE_(xaml_module_info, xaml_object)
{
    XAML_DECL_VTBL(xaml_module_info, XAML_MODULE_INFO_VTBL);
};

XAML_CLASS(xaml_module, { 0x03e2b52a, 0x7f9d, 0x4cc8, { 0x84, 0x70, 0x30, 0x0e, 0x3e, 0x6d, 0x5e, 0x5f } })

#define XAML_MODULE_VTBL(type)                  \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));  \
    XAML_METHOD(open, type, xaml_string*);      \
    XAML_METHOD(get_name, type, xaml_string**); \
    XAML_METHOD(get_info, type, xaml_module_info**)

XAML_DECL_INTERFACE_(xaml_module, xaml_object)
{
    XAML_DECL_VTBL(xaml_module, XAML_MODULE_VTBL);
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_module_new(xaml_module**) XAML_NOEXCEPT;

#endif // !XAML_META_MODULE_H
