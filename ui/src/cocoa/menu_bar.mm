#include <shared/menu_bar.hpp>
#include <xaml/ui/cocoa/window.h>
#include <xaml/ui/menu_bar.h>
#include <xaml/ui/window.h>

using namespace std;

xaml_result xaml_menu_bar_internal::draw(xaml_rectangle const& region) noexcept
{
    if (m_parent && !m_menu)
    {
        xaml_ptr<xaml_cocoa_window> native_parent;
        XAML_RETURN_IF_FAILED(m_parent->query(&native_parent));
        NSMenu* menu_bar;
        XAML_RETURN_IF_FAILED(native_parent->get_menu_bar(&menu_bar));
        m_menu = menu_bar;
        XAML_RETURN_IF_FAILED(draw_submenu());
    }
    return XAML_S_OK;
}

xaml_result xaml_menu_bar_internal::draw_visible() noexcept
{
    return XAML_S_OK;
}

xaml_result xaml_menu_bar_internal::draw_submenu() noexcept
{
    XAML_FOREACH_START(child, m_children);
    {
        xaml_ptr<xaml_control> cc;
        XAML_RETURN_IF_FAILED(child->query(&cc));
        XAML_RETURN_IF_FAILED(cc->draw({}));
    }
    XAML_FOREACH_END();
    return XAML_S_OK;
}
