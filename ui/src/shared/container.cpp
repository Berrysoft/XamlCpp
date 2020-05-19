#include <shared/container.hpp>
#include <xaml/ui/container.h>

using namespace std;

xaml_container_internal::xaml_container_internal() noexcept : xaml_control_internal() {}

xaml_container_internal::~xaml_container_internal() {}

xaml_result xaml_container_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());
    XAML_RETURN_IF_FAILED(xaml_event_new(&m_child_changed));
    return XAML_S_OK;
}

xaml_multicontainer_internal::xaml_multicontainer_internal() noexcept : xaml_control_internal() {}

xaml_multicontainer_internal::~xaml_multicontainer_internal() {}

xaml_result xaml_multicontainer_internal::init() noexcept
{
    XAML_RETURN_IF_FAILED(xaml_control_internal::init());
    XAML_RETURN_IF_FAILED(xaml_vector_new(&m_children));
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_container_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_container;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_PROP_EVENT(child, xaml_control);
    XAML_TYPE_INFO_ADD_DEF_PROP(child);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_container_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_container, "xaml/ui/container.h");
    XAML_RETURN_IF_FAILED(xaml_container_members(__info));
    return ctx->add_type(__info);
}

xaml_result XAML_CALL xaml_multicontainer_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_multicontainer;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CPROP(child, xaml_control);
    XAML_TYPE_INFO_ADD_DEF_PROP(child);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_multicontainer_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_multicontainer, "xaml/ui/container.h");
    XAML_RETURN_IF_FAILED(xaml_multicontainer_members(__info));
    return ctx->add_type(__info);
}
