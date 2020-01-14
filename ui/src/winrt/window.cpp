#include "winrt/Windows.UI.ViewManagement.h"
#include "winrt/Windows.UI.Xaml.Controls.h"
#include <shared/atomic_guard.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/window.hpp>

using namespace winrt;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::ViewManagement;

namespace xaml
{
    XAML_API window::window() : container(), m_resizable(true)
    {
        add_title_changed([this](window const&, string_view_t) { if (get_handle()) draw_title(); });
        add_location_changed([this](window const&, point) { if (get_handle() && !m_resizing) __draw({}); });
        add_size_changed([this](control const&, size) { if (get_handle() && !m_resizing) __draw({}); });
        add_resizable_changed([this](control const&, bool) { if(get_handle()) draw_resizable(); });
    }

    XAML_API window::~window() {}

    XAML_API void window::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            Page p{};
            set_handle(p);
        }
        draw_title();
        if (get_child())
        {
            draw_child();
            Page p = get_handle().as<Page>();
            p.Content(get_child()->get_handle());
        }
        draw_resizable();
    }

    XAML_API void window::__parent_redraw()
    {
        if (get_handle())
            __draw({});
    }

    XAML_API void window::draw_title()
    {
        ApplicationView::GetForCurrentView().Title(m_title);
    }

    XAML_API void window::draw_child()
    {
        get_child()->__draw({});
    }

    XAML_API void window::draw_resizable() {}

    XAML_API void window::show()
    {
        __draw({});
    }

    XAML_API void window::show(Page const& p)
    {
        set_handle(p);
        show();
    }

    XAML_API rectangle window::get_client_region() const
    {
        Page p = get_handle().as<Page>();
        auto size = p.ActualSize();
        return { 0, 0, size.x, size.y };
    }
} // namespace xaml
