#include <nowide/stackstring.hpp>
#include <shared/atomic_guard.hpp>
#include <shared/window.hpp>
#include <xaml/internal/string.hpp>

#include <Windows.Graphics.Display.h>
#include <Windows.UI.ViewManagement.h>
#include <Windows.UI.Xaml.Controls.h>
#include <Windows.UI.Xaml.h>

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::UI::Xaml;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace ABI::Windows::UI::ViewManagement;
using namespace ABI::Windows::Graphics::Display;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

xaml_window_internal::~xaml_window_internal() {}

xaml_result xaml_window_internal::draw(xaml_rectangle const&) noexcept
{
    if (!m_handle)
    {
        XAML_RETURN_IF_FAILED(ActivateInstance(HStringReference(RuntimeClass_Windows_UI_Xaml_Controls_Page).Get(), &m_handle));
        xaml_ptr<xaml_application> app;
        XAML_RETURN_IF_FAILED(xaml_application_current(&app));
        XAML_RETURN_IF_FAILED(app->window_added(static_cast<xaml_window*>(m_outer_this)));
        XAML_RETURN_IF_FAILED(draw_title());
        XAML_RETURN_IF_FAILED(draw_resizable());
    }
    XAML_RETURN_IF_FAILED(draw_size());
    XAML_RETURN_IF_FAILED(draw_child());
    XAML_RETURN_IF_FAILED(draw_menu_bar());
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_size() noexcept
{
    xaml_atomic_guard guard{ m_resizing };
    if (!guard.test_and_set())
    {
        return xaml_control_internal::draw_size();
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_title() noexcept
{
    nowide::wstackstring title;
    XAML_RETURN_IF_FAILED(to_wstring(m_title, &title));
    ComPtr<IApplicationViewStatics2> appview_static;
    XAML_RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_ViewManagement_ApplicationView).Get(), &appview_static));
    ComPtr<IApplicationView> appview;
    XAML_RETURN_IF_FAILED(appview_static->GetForCurrentView(&appview));
    return appview->put_Title(HStringReference(title.data()).Get());
}

xaml_result xaml_window_internal::draw_child() noexcept
{
    if (m_child)
    {
        xaml_rectangle region;
        XAML_RETURN_IF_FAILED(get_client_region(&region));
        return m_child->draw(region);
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_resizable() noexcept
{
    return XAML_S_OK;
}

xaml_result xaml_window_internal::draw_menu_bar() noexcept
{
    if (m_menu_bar)
    {
        XAML_RETURN_IF_FAILED(m_menu_bar->set_parent(static_cast<xaml_control*>(m_outer_this)));
        XAML_RETURN_IF_FAILED(m_menu_bar->draw({}));
    }
    return XAML_S_OK;
}

xaml_result xaml_window_internal::show() noexcept
{
    XAML_RETURN_IF_FAILED(draw({}));
    return set_is_visible(true);
}

xaml_result xaml_window_internal::close() noexcept
{
    ComPtr<IWindowStatics> window_statics;
    XAML_RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_UI_Xaml_Window).Get(), &window_statics));
    ComPtr<IWindow> window;
    XAML_RETURN_IF_FAILED(window_statics->get_Current(&window));
    return window->Close();
}

xaml_result xaml_window_internal::hide() noexcept
{
    return set_is_visible(false);
}

xaml_result xaml_window_internal::get_dpi(double* pvalue) noexcept
{
    ComPtr<IDisplayInformationStatics> info_statics;
    XAML_RETURN_IF_FAILED(GetActivationFactory(HStringReference(RuntimeClass_Windows_Graphics_Display_DisplayInformation).Get(), &info_statics));
    ComPtr<IDisplayInformation> info;
    XAML_RETURN_IF_FAILED(info_statics->GetForCurrentView(&info));
    ComPtr<IDisplayInformation3> info3;
    XAML_RETURN_IF_FAILED(info.As(&info3));
    ComPtr<IReference<double>> dpi;
    XAML_RETURN_IF_FAILED(info3->get_DiagonalSizeInInches(&dpi));
    if (XAML_FAILED(dpi->get_Value(pvalue)))
    {
        *pvalue = 96.0;
    }
    return XAML_S_OK;
}
