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
        XAML_RETURN_IF_FAILED(draw_impl(region, [this](xaml_control* c, xaml_rectangle const& subrect) -> xaml_result {
            xaml_ptr<xaml_cocoa_control> native_control;
            if (XAML_SUCCEEDED(c->query(&native_control)))
            {
                NSView* handle;
                XAML_RETURN_IF_FAILED(native_control->get_handle(&handle));
                if (handle != m_handle)
                {
                    xaml_margin margin;
                    XAML_RETURN_IF_FAILED(c->get_margin(&margin));
                    xaml_rectangle subreal = subrect - margin;
                    if (!m_put_map[c])
                    {
                        [m_handle addSubview:handle];
                        m_put_map[c] = true;
                    }
                }
            }
            return XAML_S_OK;
        }));
    }
    return XAML_S_OK;
}
