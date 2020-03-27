#ifndef XAML_UI_CONTROLS_MENU_BAR_HPP
#define XAML_UI_CONTROLS_MENU_BAR_HPP

#include <xaml/ui/container.hpp>

namespace xaml
{
    struct native_menu_bar;

    class window;
    class menu_bar;

    template <>
    struct type_guid<menu_bar>
    {
        static constexpr guid value{ 0x6e6a5fba, 0x5a9c, 0x4771, { 0xb5, 0x2a, 0x83, 0xe7, 0x61, 0xa0, 0x67, 0x46 } };
    };

    class menu_bar : public multicontainer
    {
    public:
        META_CLASS_IMPL(multicontainer)

    public:
        XAML_UI_API menu_bar();
        XAML_UI_API ~menu_bar();

    public:
        using native_menu_type = std::shared_ptr<native_menu_bar>;

    private:
        native_menu_type m_menu{ nullptr };

    public:
        native_menu_type get_menu() const noexcept { return m_menu; }

    protected:
        void set_menu(native_menu_type value) { m_menu = value; }

    private:
        std::weak_ptr<window> m_parent_window{};

    public:
        std::weak_ptr<window> get_parent_window() const noexcept { return m_parent_window; }
        void set_parent_window(std::weak_ptr<window> value) { m_parent_window = value; }

    public:
        XAML_UI_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_API virtual void draw_submenu();

#ifndef XAML_UI_GTK3
        XAML_UI_API void draw_visible() override;
#endif // !XAML_UI_GTK3

    public:
#define ADD_MENU_BAR_MEMBERS() ADD_MULTICONTAINER_MEMBERS()

        REGISTER_CLASS_DECL(xaml, menu_bar, "xaml/ui")
        {
            ADD_CTOR_DEF();
            ADD_MENU_BAR_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_MENU_BAR_HPP
