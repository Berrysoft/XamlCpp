#include <shared/menu_bar.hpp>
#include <xaml/ui/menu_bar.h>

xaml_result XAML_CALL xaml_menu_bar_new(xaml_menu_bar** ptr) noexcept
{
    return xaml_object_init<xaml_menu_bar_impl>(ptr);
}

xaml_result XAML_CALL xaml_menu_bar_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_menu_bar;
    XAML_RETURN_IF_FAILED(xaml_container_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_menu_bar_new);
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_menu_bar_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_menu_bar, "xaml/ui/menu_bar.h");
    XAML_RETURN_IF_FAILED(xaml_menu_bar_members(__info.get()));
    return ctx->add_type(__info.get());
}
