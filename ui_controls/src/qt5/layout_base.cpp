#include <shared/layout_base.hpp>
#include <xaml/ui/qt5/control.hpp>

xaml_result xaml_layout_base_internal::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        xaml_ptr<xaml_qt5_control> native_parent;
        XAML_RETURN_IF_FAILED(m_parent->query(&native_parent));
        m_handle = native_parent->get_handle();
        return draw_impl(region, {});
    }
    return XAML_S_OK;
}
