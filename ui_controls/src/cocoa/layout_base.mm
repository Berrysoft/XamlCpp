#include <shared/layout_base.hpp>
#include <xaml/ui/controls/layout_base.h>

using namespace std;

xaml_result xaml_layout_base_internal::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent)
    {
        xaml_ptr<xaml_cocoa_control> native_parent;
        XAML_RETURN_IF_FAILED(m_parent->query(&native_parent));
        NSView* parent_handle;
        XAML_RETURN_IF_FAILED(native_parent->get_handle(&parent_handle));
        m_handle = parent_handle;
        XAML_RETURN_IF_FAILED(draw_impl(region, {}));
    }
    return XAML_S_OK;
}
