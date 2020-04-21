#include <shared/container.hpp>
#include <xaml/ui/container.h>

using namespace std;

xaml_result xaml_container_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_container;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_PROP_EVENT(child);
    XAML_TYPE_INFO_ADD_DEF_PROP(child);
    return XAML_S_OK;
}

xaml_result xaml_container_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_container, "xaml/ui/container.h");
    XAML_RETURN_IF_FAILED(xaml_container_members(__info.get()));
    return ctx->add_type(__info.get());
}

xaml_result xaml_multicontainer_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_multicontainer;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CPROP(child);
    XAML_TYPE_INFO_ADD_DEF_PROP(child);
    return XAML_S_OK;
}

xaml_result xaml_multicontainer_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_multicontainer, "xaml/ui/container.h");
    XAML_RETURN_IF_FAILED(xaml_multicontainer_members(__info.get()));
    return ctx->add_type(__info.get());
}
