#include <shared/progress.hpp>
#include <xaml/ui/controls/progress.h>

using namespace std;

xaml_result xaml_progress_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_value_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_indeterminate_changed));

    int32_t token;
    XAML_RETURN_IF_FAILED((m_value_changed->add(
        [this](xaml_object*, int32_t) noexcept -> xaml_result {
            if (m_handle) XAML_RETURN_IF_FAILED(draw_progress());
            return XAML_S_OK;
        },
        &token)));
    XAML_RETURN_IF_FAILED((m_is_indeterminate_changed->add(
        [this](xaml_object*, bool) noexcept -> xaml_result {
            if (m_handle) XAML_RETURN_IF_FAILED(draw_indeterminate());
            return XAML_S_OK;
        },
        &token)));

#ifdef XAML_UI_GTK3
    XAML_RETURN_IF_FAILED(xaml_timer_new_interval(100ms, &m_pulse_timer));
    {
        xaml_ptr<xaml_delegate<xaml_object, xaml_event_args>> callback;
        XAML_RETURN_IF_FAILED((xaml_delegate_new(xaml_mem_fn(&xaml_progress_internal::on_pulse, this), &callback)));
        XAML_RETURN_IF_FAILED(m_pulse_timer->add_tick(callback, &token));
    }
#endif // XAML_UI_GTK3

    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_progress_new(xaml_progress** ptr) noexcept
{
    return xaml_object_init<xaml_progress_impl>(ptr);
}

xaml_result XAML_CALL xaml_progress_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_progress;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_progress_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(value, int32_t);
    XAML_TYPE_INFO_ADD_PROP(minimum, int32_t);
    XAML_TYPE_INFO_ADD_PROP(maximum, int32_t);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_progress_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_progress, "xaml/ui/controls/progress.h");
    XAML_RETURN_IF_FAILED(xaml_progress_members(__info));
    return ctx->add_type(__info);
}
