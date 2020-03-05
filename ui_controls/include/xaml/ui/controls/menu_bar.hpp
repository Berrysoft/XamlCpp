#ifndef XAML_UI_CONTROLS_MENU_BAR_HPP
#define XAML_UI_CONTROLS_MENU_BAR_HPP

#include <xaml/ui/container.hpp>

namespace xaml
{
    struct native_menu_bar;

    class menu_bar : public multicontainer
    {
    public:
        XAML_UI_CONTROLS_API menu_bar();
        XAML_UI_CONTROLS_API ~menu_bar();

    public:
        using native_menu_type = std::shared_ptr<native_menu_bar>;

    private:
        native_menu_type m_menu{ nullptr };

    public:
        native_menu_type get_menu() const noexcept { return m_menu; }

    protected:
        void set_menu(native_menu_type value) { m_menu = value; }

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_CONTROLS_API virtual void draw_submenu();

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
