#include <shared/layout_base.hpp>
#include <xaml/ui/qt5/control.hpp>

xaml_result xaml_layout_base_internal::draw(xaml_rectangle const& region) noexcept
{
    xaml_ptr<xaml_element_base> parent;
    XAML_RETURN_IF_FAILED(get_parent(&parent));
    if (parent)
    {
        xaml_ptr<xaml_qt5_control> native_parent;
        XAML_RETURN_IF_FAILED(parent->query(&native_parent));
        XAML_RETURN_IF_FAILED(native_parent->get_handle(&m_handle));
        return draw_impl(region, {});
    }
    return XAML_S_OK;
}
