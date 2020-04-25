#include <shared/layout_base.hpp>
#include <xaml/ui/controls/layout_base.h>

using namespace std;

xaml_result xaml_layout_base_members(xaml_type_info_registration* __info) noexcept
{
    return xaml_multicontainer_members(__info);
}

xaml_result xaml_layout_base_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_layout_base, "xaml/ui/controls/layout_base.h");
    XAML_RETURN_IF_FAILED(xaml_layout_base_members(__info.get()));
    return ctx->add_type(__info.get());
}
