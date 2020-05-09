#include <shared/layout_base.hpp>
#include <xaml/ui/controls/layout_base.h>

using namespace std;

xaml_result xaml_layout_base_internal::draw_impl(xaml_rectangle const& region, std::function<xaml_result(xaml_control*, xaml_rectangle const&)> const&) noexcept
{
#ifdef XAML_UI_WINDOWS
    using native_control_type = xaml_win32_control;
#elif defined(XAML_UI_GTK3)
    using native_control_type = xaml_gtk3_control;
#endif // XAML_UI_WINDOWS

    XAML_FOREACH_START(c, m_children);
    {
        xaml_ptr<xaml_control> cc;
        XAML_RETURN_IF_FAILED(c->query(&cc));
        xaml_ptr<native_control_type> native_control;
        if (XAML_SUCCEEDED(cc->query(&native_control)))
        {
#ifdef XAML_UI_WINDOWS
            HWND handle;
#elif defined(XAML_UI_GTK3)
            GtkWidget* handle;
#endif // XAML_UI_WINDOWS
            XAML_RETURN_IF_FAILED(native_control->get_handle(&handle));
            if (!handle)
            {
                xaml_margin margin;
                XAML_RETURN_IF_FAILED(cc->get_margin(&margin));
                XAML_RETURN_IF_FAILED(cc->draw(xaml_rectangle{} + margin));
            }
        }
        XAML_RETURN_IF_FAILED(cc->size_to_fit());
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_layout_base_members(xaml_type_info_registration* __info) noexcept
{
    return xaml_multicontainer_members(__info);
}

xaml_result XAML_CALL xaml_layout_base_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_layout_base, "xaml/ui/controls/layout_base.h");
    XAML_RETURN_IF_FAILED(xaml_layout_base_members(__info.get()));
    return ctx->add_type(__info.get());
}

xaml_result XAML_CALL xaml_orientation_register(xaml_meta_context* ctx) noexcept
{
    XAML_ENUM_INFO_MAP_NEW();
    XAML_ENUM_INFO_ADD2(xaml_orientation, horizontal);
    XAML_ENUM_INFO_ADD2(xaml_orientation, vertical);
    XAML_ENUM_INFO_NEW(xaml_orientation, "xaml/ui/controls/layout_base.h");
    return ctx->add_type(__info.get());
}
