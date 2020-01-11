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
                for (auto& len : m_columns)
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
            Grid::SetColumn(c->get_handle(), (int32_t)grid::get_column(c));
            Grid::SetRow(c->get_handle(), (int32_t)grid::get_row(c));
        }
    }
} // namespace xaml
