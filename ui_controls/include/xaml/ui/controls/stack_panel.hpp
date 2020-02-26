#ifndef XAML_UI_CONTROLS_STACK_PANEL_HPP
#define XAML_UI_CONTROLS_STACK_PANEL_HPP

#include <xaml/ui/controls/layout_base.hpp>

namespace xaml
{
    class stack_panel : public layout_base
    {
    public:
        XAML_UI_CONTROLS_API stack_panel();
        XAML_UI_CONTROLS_API ~stack_panel() override;

    private:
        XAML_UI_CONTROLS_API void __draw_impl(rectangle const& region, std::function<void(std::shared_ptr<control>, rectangle const&)> func = {}) override;

    public:
        XAML_UI_CONTROLS_API void __size_to_fit() override;

    public:
        PROP_CONSTEXPR(orientation, orientation)

    public:
#define ADD_STACK_PANEL_MEMBERS() \
    ADD_MULTICONTAINER_MEMBERS(); \
    ADD_PROP(orientation)

        REGISTER_CLASS_DECL()
        {
            REGISTER_TYPE(xaml, stack_panel);
            ADD_CTOR_DEF();
            ADD_STACK_PANEL_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_STACK_PANEL_HPP
