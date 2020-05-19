#include <calculator.h>
#include <xaml/event.h>
#include <xaml/meta/meta_macros.h>

struct xaml_test_calculator_internal
{
    xaml_object* m_outer_this{};

    XAML_EVENT_IMPL(value_changed)
    XAML_PROP_EVENT_IMPL(value, int, int*, int);

    xaml_result XAML_CALL init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_event_new(&m_value_changed));
        return XAML_S_OK;
    }
};

struct xaml_test_calculator_impl : xaml_implement<xaml_test_calculator_impl, xaml_test_calculator, xaml_object>
{
    xaml_test_calculator_internal m_internal;

    xaml_test_calculator_impl() noexcept : xaml_implement() { m_internal.m_outer_this = this; }

    XAML_EVENT_INTERNAL_IMPL(value_changed)

    XAML_PROP_INTERNAL_IMPL(value, int*, int)

    xaml_result XAML_CALL init() noexcept { return m_internal.init(); }

    xaml_result XAML_CALL plus(int x, int y) noexcept override
    {
        return set_value(x + y);
    }

    xaml_result XAML_CALL minus(int x, int y) noexcept override
    {
        return set_value(x - y);
    }
};

xaml_result XAML_CALL xaml_test_calculator_new(xaml_test_calculator** ptr) noexcept
{
    return xaml_object_init<xaml_test_calculator_impl>(ptr);
}

xaml_result XAML_CALL xaml_test_calculator_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_test_calculator, "calculator.h");
    XAML_TYPE_INFO_ADD_CTOR(xaml_test_calculator_new);
    XAML_TYPE_INFO_ADD_METHOD(plus);
    XAML_TYPE_INFO_ADD_METHOD(minus);
    XAML_TYPE_INFO_ADD_PROP_EVENT(value, int);
    return ctx->add_type(__info.get());
}
