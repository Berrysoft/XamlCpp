#include <shared/layout_base.cpp>
#include <xaml/ui/win/dpi.h>
#include <xaml/ui/win/font_provider.h>

xaml_result xaml_layout_base_internal::draw(xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_win32_control> native_parent;
    XAML_RETURN_IF_FAILED(m_parent->query(&native_parent));
    XAML_RETURN_IF_FAILED(native_parent->get_handle(&m_handle));
    xaml_ptr<xaml_application> current_app;
    XAML_RETURN_IF_FAILED(xaml_application_current(&current_app));
    xaml_ptr<xaml_win32_font_provider> provider = current_app.query<xaml_win32_font_provider>();
    if (provider)
    {
        HFONT font;
        XAML_RETURN_IF_FAILED(provider->get_default_font(XamlGetDpiForWindow(m_handle), &font));
        return draw_impl(region, [font](xaml_control* c, xaml_rectangle const& region) -> xaml_result {
            xaml_ptr<xaml_win32_control> native_control;
            if (XAML_SUCCEEDED(c->query(&native_control)))
            {
                HWND handle;
                XAML_RETURN_IF_FAILED(native_control->get_handle(&handle));
                SendMessage(handle, WM_SETFONT, (WPARAM)font, FALSE);
            }
            return XAML_S_OK;
        });
    }
    else
    {
        return draw_impl(region, {});
    }
}
