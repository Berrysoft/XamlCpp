#include <shared/button.hpp>
#include <xaml/ui/controls/button.h>

using namespace std;

xaml_result xaml_button_new(xaml_button** ptr) noexcept
{
    return xaml_object_init<xaml_button_impl>(ptr);
}

xaml_result xaml_button_members(xaml_type_info_registration* __info) noexcept
{
    using self_type = xaml_button;
    XAML_RETURN_IF_FAILED(xaml_control_members(__info));
    XAML_TYPE_INFO_ADD_CTOR(xaml_button_new);
    XAML_TYPE_INFO_ADD_PROP_EVENT(text);
    XAML_TYPE_INFO_ADD_PROP_EVENT(is_default);
    XAML_TYPE_INFO_ADD_EVENT(click);
    XAML_TYPE_INFO_ADD_DEF_PROP(text);
    return XAML_S_OK;
}

xaml_result xaml_button_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_button, "xaml/ui/controls/button.h");
    XAML_RETURN_IF_FAILED(xaml_button_members(__info.get()));
    return ctx->add_type(__info.get());
}
