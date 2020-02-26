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
        XAML_UI_CONTROLS_API std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static void on_changed(GtkComboBox* widget, gpointer data);
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
        XAML_UI_CONTROLS_API void __size_to_fit() override;

    private:
        XAML_UI_CONTROLS_API void draw_size();
        XAML_UI_CONTROLS_API void draw_items();
        XAML_UI_CONTROLS_API void draw_text();
        XAML_UI_CONTROLS_API void draw_sel();
        XAML_UI_CONTROLS_API void draw_editable();

        EVENT(text_changed, combo_box&, std::optional<string_t> const&)
        PROP_EVENT(text, std::optional<string_t>)

        EVENT(is_editable_changed, combo_box&, bool)
        PROP_CONSTEXPR_EVENT(is_editable, bool)

    public:
#define ADD_COMBO_BOX_MEMBERS()  \
    ADD_ITEMS_BASE_MEMBERS();    \
    ADD_PROP_EVENT(is_editable); \
    ADD_PROP_EVENT(text);        \
    ADD_DEF_PROP(text)

        REGISTER_CLASS_DECL()
        {
            REGISTER_TYPE(xaml, combo_box);
            ADD_CTOR_DEF();
            ADD_COMBO_BOX_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_COMBO_BOX_HPP
