#include <shared/control.hpp>
#include <xaml/meta/enum_info.h>
#include <xaml/ui/control.h>

using namespace std;

xaml_control_internal::xaml_control_internal() noexcept : m_is_visible(true)
{
}

xaml_control_internal::~xaml_control_internal() {}

xaml_result xaml_control_internal::parent_redraw() noexcept
{
    if (m_parent)
    {
        xaml_ptr<xaml_control> control;
        if (XAML_SUCCEEDED(m_parent->query(&control)))
        {
            return control->parent_redraw();
        }
        return XAML_S_OK;
    }
    else
    {
        return draw({});
    }
}

xaml_result xaml_control_internal::init() noexcept
{
    xaml_ptr<xaml_hasher> hasher;
    XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&hasher));
    XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(hasher, &m_resources));

    XAML_RETURN_IF_FAILED(xaml_event_new(&m_parent_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_size_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_margin_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_halignment_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_valignment_changed));
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_is_visible_changed));

    std::int32_t token;
    XAML_RETURN_IF_FAILED((m_size_changed->add_noexcept<xaml_ptr<xaml_control>, xaml_size>(
        [this](xaml_ptr<xaml_control>, xaml_size) noexcept -> xaml_result {
            if (m_handle)
            {
                XAML_RETURN_IF_FAILED(draw_size());
                XAML_RETURN_IF_FAILED(parent_redraw());
            }
            return XAML_S_OK;
        },
        &token)));
    XAML_RETURN_IF_FAILED((m_is_visible_changed->add_noexcept<xaml_ptr<xaml_control>, bool>(
        [this](xaml_ptr<xaml_control>, bool) noexcept -> xaml_result {
            if (m_handle) XAML_RETURN_IF_FAILED(draw_visible());
            return XAML_S_OK;
        },
        &token)));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_control_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_control;
    XAML_RETURN_IF_FAILED(xaml_element_base_members(__info));
    XAML_TYPE_INFO_ADD_PROP_EVENT(size, xaml_size);
    XAML_TYPE_INFO_ADD_PROP(width, double);
    XAML_TYPE_INFO_ADD_PROP(height, double);
    XAML_TYPE_INFO_ADD_PROP_EVENT(margin, xaml_margin);
    XAML_TYPE_INFO_ADD_PROP_EVENT(halignment, xaml_halignment);
    XAML_TYPE_INFO_ADD_PROP_EVENT(valignment, xaml_valignment);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_control_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_control, "xaml/ui/control.h");
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_halignment_register(xaml_meta_context* ctx) noexcept
{
    XAML_ENUM_INFO_MAP_NEW();
    XAML_ENUM_INFO_ADD2(xaml_halignment, stretch);
    XAML_ENUM_INFO_ADD2(xaml_halignment, left);
    XAML_ENUM_INFO_ADD2(xaml_halignment, center);
    XAML_ENUM_INFO_ADD2(xaml_halignment, right);
    XAML_ENUM_INFO_NEW(xaml_halignment, "xaml/ui/control.h");
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_valignment_register(xaml_meta_context* ctx) noexcept
{
    XAML_ENUM_INFO_MAP_NEW();
    XAML_ENUM_INFO_ADD2(xaml_valignment, stretch);
    XAML_ENUM_INFO_ADD2(xaml_valignment, top);
    XAML_ENUM_INFO_ADD2(xaml_valignment, center);
    XAML_ENUM_INFO_ADD2(xaml_valignment, bottom);
    XAML_ENUM_INFO_NEW(xaml_valignment, "xaml/ui/control.h");
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_mouse_button_register(xaml_meta_context* ctx) noexcept
{
    XAML_ENUM_INFO_MAP_NEW();
    XAML_ENUM_INFO_ADD2(xaml_mouse_button, left);
    XAML_ENUM_INFO_ADD2(xaml_mouse_button, right);
    XAML_ENUM_INFO_ADD2(xaml_mouse_button, middle);
    XAML_ENUM_INFO_NEW(xaml_mouse_button, "xaml/ui/control.h");
    return ctx->add_type(__info);
}
