#ifndef XAML_UI_CONTROLS_MENU_ITEM_HPP
#define XAML_UI_CONTROLS_MENU_ITEM_HPP

#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    struct native_menu_item;

    class menu_item : public control
    {
    public:
        XAML_UI_CONTROLS_API menu_item();
        XAML_UI_CONTROLS_API ~menu_item();

    public:
        using native_menu_type = std::shared_ptr<native_menu_item>;

    private:
        native_menu_type m_menu{ nullptr };

    public:
        native_menu_type get_menu() const noexcept { return m_menu; }

    protected:
        void set_menu(native_menu_type value) { m_menu = value; }

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_CONTROLS_API std::optional<std::intptr_t> __wnd_proc(window_message const& msg) override;

    protected:
        XAML_UI_CONTROLS_API void __draw_impl(std::uint32_t flags);
        XAML_UI_CONTROLS_API virtual void draw_append(void* pmenu, std::uint32_t flags);
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    protected:
        static void on_activate(void* menu_item, void* data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    protected:
        XAML_UI_CONTROLS_API virtual void draw_append(void* pmenu);

    public:
        void __on_action();
#endif // XAML_UI_COCOA

    public:
        PROP_STRING(text)

        EVENT(click, menu_item&)

    public:
#define ADD_MENU_ITEM_MEMBERS() \
    ADD_CONTROL_MEMBERS();      \
    ADD_PROP(text);             \
    ADD_EVENT(click);           \
    ADD_DEF_PROP(text)

        REGISTER_CLASS_DECL(xaml, menu_item)
        {
            ADD_CTOR_DEF();
            ADD_MENU_ITEM_MEMBERS();
        }
        REGISTER_CLASS_END()
    };

    class popup_menu_item : public menu_item
    {
    public:
        XAML_UI_CONTROLS_API popup_menu_item();
        XAML_UI_CONTROLS_API ~popup_menu_item();

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_CONTROLS_API std::optional<std::intptr_t> __wnd_proc(window_message const& msg) override;

    protected:
        XAML_UI_CONTROLS_API void draw_append(void* pmenu, std::uint32_t flags) override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_COCOA
    protected:
        XAML_UI_CONTROLS_API void draw_append(void* pmenu) override;
#endif // XAML_UI_COCOA

    private:
        std::vector<std::shared_ptr<menu_item>> m_submenu{};

    public:
        XAML_UI_CONTROLS_API void add_submenu(std::shared_ptr<menu_item> const& child);
        XAML_UI_CONTROLS_API void remove_submenu(std::shared_ptr<menu_item> const& child);
        array_view<std::shared_ptr<menu_item>> get_submenu() const noexcept { return m_submenu; }

    protected:
        XAML_UI_CONTROLS_API virtual void draw_submenu();

    public:
#define ADD_POPUP_MENU_ITEM_MEMBERS() \
    ADD_MENU_ITEM_MEMBERS();          \
    ADD_COLLECTION_PROP(submenu, std::shared_ptr<menu_item>)

        REGISTER_CLASS_DECL(xaml, popup_menu_item)
        {
            ADD_CTOR_DEF();
            ADD_POPUP_MENU_ITEM_MEMBERS();
        }
        REGISTER_CLASS_END()
    };

    class check_menu_item : public menu_item
    {
    public:
        XAML_UI_CONTROLS_API check_menu_item();
        XAML_UI_CONTROLS_API ~check_menu_item();

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_CONTROLS_API virtual void draw_checked();

    public:
        PROP_CONSTEXPR_EVENT(is_checked, bool)
        EVENT(is_checked_changed, check_menu_item&, bool)

    public:
#define ADD_CHECK_MENU_ITEM_MEMBERS() \
    ADD_MENU_ITEM_MEMBERS();          \
    ADD_PROP_EVENT(is_checked)

        REGISTER_CLASS_DECL(xaml, check_menu_item)
        {
            ADD_CTOR_DEF();
            ADD_CHECK_MENU_ITEM_MEMBERS();
        }
        REGISTER_CLASS_END()
    };

    class radio_menu_item : public menu_item
    {
    public:
        XAML_UI_CONTROLS_API radio_menu_item();
        XAML_UI_CONTROLS_API ~radio_menu_item();

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_CONTROLS_API virtual void draw_checked();
        XAML_UI_CONTROLS_API virtual void draw_group();

    public:
        PROP_CONSTEXPR_EVENT(is_checked, bool)
        EVENT(is_checked_changed, radio_menu_item&, bool)

        PROP_STRING(group)

    public:
#define ADD_RADIO_MENU_ITEM_MEMBERS() \
    ADD_MENU_ITEM_MEMBERS();          \
    ADD_PROP_EVENT(is_checked);       \
    ADD_PROP(group)

        REGISTER_CLASS_DECL(xaml, radio_menu_item)
        {
            ADD_CTOR_DEF();
            ADD_RADIO_MENU_ITEM_MEMBERS();
        }
        REGISTER_CLASS_END()
    };

    class separator_menu_item : public menu_item
    {
    public:
        XAML_UI_CONTROLS_API separator_menu_item();
        XAML_UI_CONTROLS_API ~separator_menu_item();

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

#ifdef XAML_UI_COCOA
    protected:
        XAML_UI_CONTROLS_API void draw_append(void* pmenu) override;
#endif // XAML_UI_COCOA

    public:
#define ADD_SEPARATOR_MENU_ITEM_MEMBERS() ADD_MENU_ITEM_MEMBERS()

        REGISTER_CLASS_DECL(xaml, separator_menu_item)
        {
            ADD_CTOR_DEF();
            ADD_SEPARATOR_MENU_ITEM_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_MENU_ITEM_HPP
