#ifndef XAML_UI_CONTROLS_SHARED_LAYOUT_BASE_HPP
#define XAML_UI_CONTROLS_SHARED_LAYOUT_BASE_HPP

#include <shared/container.hpp>
#include <xaml/ui/controls/layout_base.h>

#ifdef XAML_UI_WINDOWS
#include <xaml/ui/win/control.h>
#endif // XAML_UI_WINDOWS

template <typename T, typename... Base>
struct xaml_layout_base_implement : xaml_multicontainer_implement<T, Base..., xaml_layout_base>
{
    virtual xaml_result draw_impl(xaml_rectangle const& region, std::function<xaml_result(xaml_control*, xaml_rectangle const&)>) noexcept
    {
        for (auto c : m_children)
        {
            xaml_ptr<xaml_control> cc;
            XAML_RETURN_IF_FAILED(c->query(&cc));
#ifdef XAML_UI_WINDOWS
            xaml_ptr<xaml_win32_control> native_control;
#endif // XAML_UI_WINDOWS
            if (XAML_SUCCEEDED(cc->query(&native_control)))
            {
#ifdef XAML_UI_WINDOWS
                HWND handle;
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
        return XAML_S_OK;
    }

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL draw(xaml_rectangle const& region) noexcept override
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
#endif // XAML_UI_WINDOWS
};

#endif // !XAML_UI_CONTROLS_SHARED_LAYOUT_BASE_HPP
