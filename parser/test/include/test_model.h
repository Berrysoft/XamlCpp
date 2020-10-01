#ifndef XAML_PARSER_TEST_MODEL_H
#define XAML_PARSER_TEST_MODEL_H

#include <xaml/meta/meta_context.h>
#include <xaml/meta/meta_macros.h>
#include <xaml/observable_vector.h>

#ifndef xaml_enumerator_1__xaml_object_defined
    #define xaml_enumerator_1__xaml_object_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_object))
#endif // !xaml_enumerator_1__xaml_object_defined

#ifndef xaml_vector_changed_args_1__xaml_object_defined
    #define xaml_vector_changed_args_1__xaml_object_defined
XAML_VECTOR_CHANGED_ARGS_1_TYPE(XAML_T_O(xaml_object))
#endif // !xaml_vector_changed_args_1__xaml_object_defined

#ifndef xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_object_defined
    #define xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_object_defined
XAML_DELEGATE_2_TYPE(XAML_T_O(xaml_object), XAML_T_O(xaml_vector_changed_args_1__xaml_object))
#endif // !xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_object_defined

#ifndef xaml_observable_vector_1__xaml_object_defined
    #define xaml_observable_vector_1__xaml_object_defined
XAML_OBSERVABLE_VECTOR_1_TYPE(XAML_T_O(xaml_object))
#endif // !xaml_observable_vector_1__xaml_object_defined

XAML_CLASS(xaml_test_model, { 0x16fd8017, 0x13f2, 0x4fd4, { 0xaa, 0x86, 0x7c, 0x28, 0x54, 0x2a, 0x7c, 0xf7 } })

#define XAML_TEST_MODEL_VTBL(type)                            \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                \
    XAML_PROP(text, type, xaml_string**, xaml_string*);       \
    XAML_EVENT(text_changed, type, xaml_object, xaml_string); \
    XAML_PROP(items, type, XAML_OBSERVABLE_VECTOR_1_NAME(xaml_object)**, XAML_OBSERVABLE_VECTOR_1_NAME(xaml_object)*)

XAML_DECL_INTERFACE_(xaml_test_model, xaml_object)
{
    XAML_DECL_VTBL(xaml_test_model, XAML_TEST_MODEL_VTBL);
};

EXTERN_C xaml_result XAML_CALL xaml_test_model_new(xaml_test_model**) XAML_NOEXCEPT;
EXTERN_C xaml_result XAML_CALL xaml_test_model_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_PARSER_TEST_MODEL_H
