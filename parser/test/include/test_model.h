#ifndef XAML_PARSER_TEST_MODEL_H
#define XAML_PARSER_TEST_MODEL_H

#include <xaml/meta/meta_context.h>
#include <xaml/meta/meta_macros.h>

XAML_CLASS(xaml_test_model, { 0x16fd8017, 0x13f2, 0x4fd4, { 0xaa, 0x86, 0x7c, 0x28, 0x54, 0x2a, 0x7c, 0xf7 } })

#define XAML_TEST_MODEL_VTBL(type)                      \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));          \
    XAML_PROP(text, type, xaml_string**, xaml_string*); \
    XAML_EVENT(text_changed, type)

XAML_DECL_INTERFACE_(xaml_test_model, xaml_object)
{
    XAML_DECL_VTBL(xaml_test_model, XAML_TEST_MODEL_VTBL);
};

EXTERN_C xaml_result XAML_CALL xaml_test_model_new(xaml_test_model**) XAML_NOEXCEPT;
EXTERN_C xaml_result XAML_CALL xaml_test_model_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_PARSER_TEST_MODEL_H
