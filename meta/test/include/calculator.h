#ifndef XAML_TEST_CALCULATOR_H
#define XAML_TEST_CALCULATOR_H

#include <xaml/delegate.h>
#include <xaml/meta/meta_context.h>
#include <xaml/object.h>

XAML_CLASS(xaml_test_calculator, { 0x2662320c, 0xb748, 0x435d, { 0xae, 0xfb, 0xc5, 0x2e, 0xed, 0xb9, 0xa9, 0xb5 } })

template <>
struct xaml_base<xaml_test_calculator>
{
    using type = xaml_object;
};

struct XAML_NOVTBL xaml_test_calculator : xaml_object
{
    virtual xaml_result XAML_CALL get_value(int*) noexcept = 0;
    virtual xaml_result XAML_CALL set_value(int) noexcept = 0;
    virtual xaml_result XAML_CALL add_value_changed(xaml_delegate<xaml_object, std::int32_t>*, std::int32_t*) noexcept = 0;
    virtual xaml_result XAML_CALL remove_value_changed(std::int32_t) noexcept = 0;
    virtual xaml_result XAML_CALL plus(int, int) noexcept = 0;
    virtual xaml_result XAML_CALL minus(int, int) noexcept = 0;
};

EXTERN_C xaml_result XAML_CALL xaml_test_calculator_new(xaml_test_calculator**) XAML_NOEXCEPT;
EXTERN_C xaml_result XAML_CALL xaml_test_calculator_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_TEST_CALCULATOR_H
