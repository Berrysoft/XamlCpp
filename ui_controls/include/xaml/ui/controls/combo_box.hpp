#ifndef XAML_UI_CONTROLS_COMBO_BOX_HPP
#define XAML_UI_CONTROLS_COMBO_BOX_HPP

#include <xaml/meta/default_property.hpp>
#include <xaml/ui/controls/items_base.hpp>

namespace xaml
{
    class combo_box : public items_base<string_t>
    {
    public:
        XAML_UI_CONTROLS_API combo_box();
        XAML_UI_CONTROLS_API ~combo_box() override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_CONTROLS_API std::optional<std::intptr_t> __wnd_proc(window_message const& msg) override;
        XAML_UI_CONTROLS_API void __size_to_fit() override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static void on_changed(void* widget, void* data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_changed();
#endif // XAML_UI_COCOA

    protected:
        void insert_item(std::size_t index, string_t const& value) override;
        void remove_item(std::size_t index) override;
        void clear_items() override;
        void replace_item(std::size_t index, string_t const& value) override;

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_CONTROLS_API virtual void draw_items();
        XAML_UI_CONTROLS_API virtual void draw_text();
        XAML_UI_CONTROLS_API virtual void draw_sel();
        XAML_UI_CONTROLS_API virtual void draw_editable();

        EVENT(text_changed, std::reference_wrapper<combo_box>, std::optional<string_view_t>)
        PROP_EVENT(text, std::optional<string_t>)

        EVENT(is_editable_changed, std::reference_wrapper<combo_box>, bool)
        PROP_CONSTEXPR_EVENT(is_editable, bool)

    public:
#define ADD_COMBO_BOX_MEMBERS()  \
    ADD_ITEMS_BASE_MEMBERS();    \
    ADD_PROP_EVENT(is_editable); \
    ADD_PROP_EVENT(text);        \
    ADD_DEF_PROP(text)

        REGISTER_CLASS_DECL(xaml, combo_box, "xaml/ui/controls")
        {
            ADD_CTOR_DEF();
            ADD_COMBO_BOX_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_COMBO_BOX_HPP
