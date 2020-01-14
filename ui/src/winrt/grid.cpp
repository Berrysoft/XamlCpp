#include "winrt/Windows.Foundation.Collections.h"
#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/Windows.UI.Xaml.h"
#include <winrt/drawing.hpp>
#include <xaml/ui/grid.hpp>

using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;

namespace xaml
{
    static GridLength get_GridLength(grid_length const& len)
    {
        switch (len.layout)
        {
        case grid_layout::abs:
            return { len.value, GridUnitType::Pixel };
        case grid_layout::star:
            return { len.value, GridUnitType::Star };
        case grid_layout::compact:
            return { len.value, GridUnitType::Auto };
        default:
            return {};
        }
    }

    XAML_API void grid::__draw(rectangle const& region)
    {
        bool new_run = !get_handle();
        if (!get_handle())
        {
            Grid g{};
            {
                auto defs = g.ColumnDefinitions();
                for (auto& len : m_columns)
                {
                    ColumnDefinition def{};
                    def.Width(get_GridLength(len));
                    defs.Append(def);
                }
            }
            {
                auto defs = g.RowDefinitions();
                for (auto& len : m_rows)
                {
                    RowDefinition def{};
                    def.Height(get_GridLength(len));
                    defs.Append(def);
                }
            }
            set_handle(g);
        }
        Grid g = get_handle().as<Grid>();
        g.Margin(get_Thickness(get_margin()));
        for (auto& c : m_children)
        {
            c->__draw({});
            if (new_run) g.Children().Append(c->get_handle());
            auto cc = c->get_handle();
            switch (c->get_halignment())
            {
            case halignment_t::stretch:
                cc.HorizontalAlignment(HorizontalAlignment::Stretch);
                break;
            case halignment_t::center:
                cc.HorizontalAlignment(HorizontalAlignment::Center);
                break;
            case halignment_t::right:
                cc.HorizontalAlignment(HorizontalAlignment::Right);
                break;
            default:
                cc.HorizontalAlignment(HorizontalAlignment::Left);
                break;
            }
            switch (c->get_valignment())
            {
            case valignment_t::stretch:
                cc.VerticalAlignment(VerticalAlignment::Stretch);
                break;
            case valignment_t::center:
                cc.VerticalAlignment(VerticalAlignment::Center);
                break;
            case valignment_t::bottom:
                cc.VerticalAlignment(VerticalAlignment::Bottom);
                break;
            default:
                cc.VerticalAlignment(VerticalAlignment::Top);
                break;
            }
            Grid::SetColumn(c->get_handle(), (int32_t)grid::get_column(c));
            Grid::SetRow(c->get_handle(), (int32_t)grid::get_row(c));
        }
    }
} // namespace xaml
