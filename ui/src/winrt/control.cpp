#include <shared/control.hpp>
#include <xaml/ui/drawing_conv.hpp>

#include <Windows.UI.Xaml.h>

using namespace ABI::Windows::Foundation;
using namespace ABI::Windows::Foundation::Collections;
using namespace ABI::Windows::UI::Xaml;
using namespace ABI::Windows::UI::Xaml::Controls;
using namespace Microsoft::WRL;

template <typename T2 = IInspectable, typename T>
static xaml_result set_single_child(ComPtr<T> const& parent, ComPtr<IFrameworkElement> const& child) noexcept
{
    ComPtr<T2> c;
    XAML_RETURN_IF_FAILED(child.As(&c));
    return parent->put_Content(c.Get());
}

template <typename TC, typename T2, typename T>
static xaml_result add_child(ComPtr<T> const& parent, ComPtr<IFrameworkElement> const& child) noexcept
{
    ComPtr<IVector<TC*>> children;
    XAML_RETURN_IF_FAILED(parent->get_Children(&children));
    ComPtr<T2> c;
    XAML_RETURN_IF_FAILED(child.As(&c));
    return children->Append(c.Get());
}

static xaml_result set_child(ComPtr<IFrameworkElement> const& parent, ComPtr<IFrameworkElement> const& child) noexcept
{
    {
        ComPtr<IContentControl> content_control;
        if (XAML_SUCCEEDED(parent.As(&content_control)))
        {
            return set_single_child(content_control, child);
        }
    }
    {
        ComPtr<IUserControl> user_control;
        if (XAML_SUCCEEDED(parent.As(&user_control)))
        {
            return set_single_child<IUIElement>(user_control, child);
        }
    }
    {
        ComPtr<IPanel> panel;
        if (XAML_SUCCEEDED(parent.As(&panel)))
        {
            return add_child<UIElement, IUIElement>(panel, child);
        }
    }
    return XAML_S_OK;
}

xaml_result xaml_control_internal::set_rect(xaml_rectangle const& region) noexcept
{
    xaml_rectangle real = region - m_margin;
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent)
    {
        xaml_ptr<xaml_winrt_control> native_parent;
        if (XAML_SUCCEEDED(parent->query(&native_parent)))
        {
            ComPtr<IFrameworkElement> parent_handle;
            XAML_RETURN_IF_FAILED(native_parent->get_handle(&parent_handle));
            XAML_RETURN_IF_FAILED(set_child(parent_handle, m_handle));
        }
    }
    XAML_RETURN_IF_FAILED(set_size_noevent({ real.width, real.height }));
    XAML_RETURN_IF_FAILED(draw_size());
    return XAML_S_OK;
}

xaml_result xaml_control_internal::size_to_fit() noexcept
{
    ComPtr<IUIElement> element;
    XAML_RETURN_IF_FAILED(m_handle.As(&element));
    Size s;
    XAML_RETURN_IF_FAILED(element->get_DesiredSize(&s));
    return set_size_noevent(xaml_from_native(s));
}

xaml_result xaml_control_internal::draw_size() noexcept
{
    XAML_RETURN_IF_FAILED(m_handle->put_Width(m_size.width));
    XAML_RETURN_IF_FAILED(m_handle->put_Height(m_size.height));
    return XAML_S_OK;
}

xaml_result xaml_control_internal::draw_visible() noexcept
{
    ComPtr<IUIElement> element;
    XAML_RETURN_IF_FAILED(m_handle.As(&element));
    return element->put_Visibility(m_is_visible ? Visibility_Visible : Visibility_Collapsed);
}
