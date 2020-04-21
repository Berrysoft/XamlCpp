#include <shared/control.hpp>
#include <xaml/ui/control.h>

using namespace std;

xaml_result xaml_control_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_control, "xaml/ui/control.h");
    XAML_TYPE_INFO_ADD_PROP_EVENT(size);
    XAML_TYPE_INFO_ADD_PROP(width);
    XAML_TYPE_INFO_ADD_PROP(height);
    XAML_TYPE_INFO_ADD_PROP_EVENT(margin);
    XAML_TYPE_INFO_ADD_PROP_EVENT(halignment);
    XAML_TYPE_INFO_ADD_PROP_EVENT(valignment);
    return ctx->add_type(__info.get());
}
