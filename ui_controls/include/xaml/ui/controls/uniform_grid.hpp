#ifndef XAML_UI_CONTROLS_UNIFORM_GRID_HPP
#define XAML_UI_CONTROLS_UNIFORM_GRID_HPP

#include <xaml/ui/controls/layout_base.hpp>

namespace xaml
{
    class uniform_grid : public layout_base
    {
    public:
        XAML_UI_CONTROLS_API uniform_grid();
        XAML_UI_CONTROLS_API ~uniform_grid() override;

    private:
        XAML_UI_CONTROLS_API void __draw_impl(rectangle const& region, std::function<void(std::shared_ptr<control>, rectangle const&)> func = {}) override;

    public:
        XAML_UI_CONTROLS_API void __size_to_fit() override;

    public:
        PROP_CONSTEXPR(columns, std::size_t)
        PROP_CONSTEXPR(rows, std::size_t)
        PROP_CONSTEXPR(orientation, orientation)

    public:
#define ADD_UNIFORM_GRID_MEMBERS() \
    ADD_MULTICONTAINER_MEMBERS();  \
    ADD_PROP(columns);             \
    ADD_PROP(rows);                \
    ADD_PROP(orientation)

        REGISTER_CLASS_DECL(xaml, uniform_grid, "xaml/ui/controls")
        {
            ADD_CTOR_DEF();
            ADD_UNIFORM_GRID_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_UNIFORM_GRID_HPP
