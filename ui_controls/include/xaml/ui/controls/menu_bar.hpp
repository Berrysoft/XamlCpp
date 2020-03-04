#ifndef XAML_UI_CONTROLS_MENU_BAR_HPP
#define XAML_UI_CONTROLS_MENU_BAR_HPP

#include <xaml/ui/container.hpp>

namespace xaml
{
    class menu_bar : public multicontainer
    {
    public:
        XAML_UI_CONTROLS_API menu_bar();
        XAML_UI_CONTROLS_API ~menu_bar();

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    public:
#define ADD_MENU_BAR_MEMBERS() ADD_MULTICONTAINER_MEMBERS()

        REGISTER_CLASS_DECL(xaml, menu_bar)
        {
            ADD_CTOR_DEF();
            ADD_MENU_BAR_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_MENU_BAR_HPP
