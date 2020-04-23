#include <shared/menu_bar.hpp>
#include <xaml/ui/menu_bar.h>

xaml_result xaml_menu_bar_new(xaml_menu_bar** ptr) noexcept
{
    return xaml_object_init<xaml_menu_bar_impl>(ptr);
}

xaml_result xaml_menu_bar_members(xaml_type_info_registration* __info) noexcept
{
    return xaml_container_members(__info);
}

xaml_result xaml_menu_bar_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_menu_bar, "xaml/ui/menu_bar.h");
    XAML_RETURN_IF_FAILED(xaml_menu_bar_members(__info.get()));
    return ctx->add_type(__info.get());
}
