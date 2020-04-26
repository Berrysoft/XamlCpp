#ifndef XAML_UI_CONTROLS_UNIFORM_GRID_HPP
#define XAML_UI_CONTROLS_UNIFORM_GRID_HPP

#include <xaml/ui/controls/layout_base.hpp>

namespace xaml
{
    class uniform_grid;

    template <>
    struct type_guid<uniform_grid>
    {
        static constexpr guid value{ 0x1963c67d, 0xdc81, 0x40dd, { 0x91, 0x2e, 0xa6, 0xa9, 0x44, 0x48, 0xf2, 0x37 } };
    };

    class uniform_grid : public layout_base
    {
    public:
        META_CLASS_IMPL(layout_base)

    public:
        XAML_UI_CONTROLS_API uniform_grid();
        XAML_UI_CONTROLS_API ~uniform_grid() override;

    private:
        XAML_UI_CONTROLS_API void __draw_impl(rectangle const& region, std::function<void(std::shared_ptr<control>, rectangle const&)> func = {}) override;

    public:
        XAML_UI_CONTROLS_API void __size_to_fit() override;

    public:
        PROP_CONSTEXPR(columns, std::int32_t)
        PROP_CONSTEXPR(rows, std::int32_t)
        PROP_CONSTEXPR(orientation, orientation)

    public:
#define ADD_UNIFORM_GRID_MEMBERS() \
    ADD_MULTICONTAINER_MEMBERS();  \
    ADD_PROP(columns);             \
    ADD_PROP(rows);                \
    ADD_PROP(orientation)

        REGISTER_CLASS_DECL(xaml, uniform_grid, "xaml/ui/controls")
        {
            ADD_CTOR();
            ADD_UNIFORM_GRID_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_UNIFORM_GRID_HPP
