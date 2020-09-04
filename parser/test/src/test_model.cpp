#include <test_model.h>
#include <xaml/event.h>
#include <xaml/weak_reference.h>

#define m_outer_this this

struct xaml_test_model_impl : xaml_weak_implement<xaml_test_model_impl, xaml_test_model, xaml_object>
{
    XAML_EVENT_IMPL(text_changed)
    XAML_PROP_STRING_EVENT_IMPL(text)
    XAML_PROP_PTR_IMPL(items, xaml_observable_vector)

    xaml_result XAML_CALL init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_event_new(&m_text_changed));
        XAML_RETURN_IF_FAILED(xaml_observable_vector_new(&m_items));
        return XAML_S_OK;
    }
};

#undef m_outer_this

xaml_result XAML_CALL xaml_test_model_new(xaml_test_model** ptr) noexcept
{
    return xaml_object_init<xaml_test_model_impl>(ptr);
}

xaml_result XAML_CALL xaml_test_model_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_test_model, "test_model.h");
    XAML_TYPE_INFO_ADD_CTOR(xaml_test_model_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(text, xaml_string);
    XAML_TYPE_INFO_ADD_PROP(items, xaml_observable_vector);
    return ctx->add_type(__info);
}
