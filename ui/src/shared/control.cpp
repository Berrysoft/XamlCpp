#include <shared/control.hpp>
#include <xaml/ui/control.h>

using namespace std;

xaml_result xaml_control_internal::parent_redraw() noexcept
{
    if (m_parent)
        return m_parent->parent_redraw();
    else
        return draw({});
}

xaml_result xaml_control_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_parent_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_size_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_margin_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_halignment_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_valignment_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_visible_changed));

    std::int32_t token;
    XAML_RETURN_IF_FAILED((m_size_changed->add_noexcept<xaml_ptr<xaml_control>, xaml_size>(
        [this](xaml_ptr<xaml_control>, xaml_size) -> xaml_result {
            if (m_handle)
            {
                XAML_RETURN_IF_FAILED(draw_size());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));
    XAML_RETURN_IF_FAILED((m_is_visible_changed->add_noexcept<xaml_ptr<xaml_control>, bool>(
        [this](xaml_ptr<xaml_control>, bool) -> xaml_result {
            if (m_handle) XAML_RETURN_IF_FAILED(draw_visible());
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_control_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_control;
    XAML_TYPE_INFO_ADD_PROP_EVENT(size);
    XAML_TYPE_INFO_ADD_PROP(width);
    XAML_TYPE_INFO_ADD_PROP(height);
    XAML_TYPE_INFO_ADD_PROP_EVENT(margin);
    XAML_TYPE_INFO_ADD_PROP_EVENT(halignment);
    XAML_TYPE_INFO_ADD_PROP_EVENT(valignment);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_control_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_control, "xaml/ui/control.h");
    XAML_RETURN_IF_FAILED(xaml_control_members(__info.get()));
    return ctx->add_type(__info.get());
}
