#ifndef XAML_TEST_CONVERTER_H
#define XAML_TEST_CONVERTER_H

#include <xaml/converter.h>
#include <xaml/meta/meta_context.h>

XAML_CLASS(xaml_test_converter, { 0xbd289f3b, 0x5415, 0x4e49, { 0xb0, 0x88, 0x75, 0x25, 0x7e, 0x92, 0x5d, 0x19 } })

#define XAML_TEST_CONVERTER_VTBL(type) XAML_VTBL_INHERIT(XAML_CONVERTER_VTBL(type))

XAML_DECL_INTERFACE_(xaml_test_converter, xaml_converter)
{
    XAML_DECL_VTBL(xaml_test_converter, XAML_TEST_CONVERTER_VTBL);
};

EXTERN_C xaml_result XAML_CALL xaml_test_converter_new(xaml_test_converter**) XAML_NOEXCEPT;
EXTERN_C xaml_result XAML_CALL xaml_test_converter_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_TEST_CONVERTER2_H
