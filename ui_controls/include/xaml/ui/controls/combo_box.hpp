#ifndef XAML_UI_CONTROLS_COMBO_BOX_HPP
#define XAML_UI_CONTROLS_COMBO_BOX_HPP

#include <vector>
#include <xaml/array_view.hpp>
#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class combo_box : public control
    {
    public:
        XAML_UI_CONTROLS_API combo_box();
        XAML_UI_CONTROLS_API ~combo_box() override;

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;
        XAML_UI_CONTROLS_API void __size_to_fit() override;

    private:
#if defined(XAML_UI_WINDOWS) || defined(XAML_UI_GTK3)
        XAML_UI_CONTROLS_API void draw_create();
#endif // XAML_UI_WINDOWS || XAML_UI_GTK3

        XAML_UI_CONTROLS_API void draw_size();
        XAML_UI_CONTROLS_API void draw_items();
        XAML_UI_CONTROLS_API void draw_text();
        XAML_UI_CONTROLS_API void draw_sel();
        XAML_UI_CONTROLS_API void draw_editable();

    private:
        std::vector<string_t> m_items{};

    public:
        array_view<string_t> get_items() const { return m_items; }
        void set_items(array_view<string_t> value) { m_items = value; }

    public:
        EVENT(text_changed, combo_box&, std::optional<string_t> const&)
        PROP_EVENT(text, std::optional<string_t>)

        EVENT(sel_id_changed, combo_box&, std::size_t)
        PROP_CONSTEXPR_EVENT(sel_id, std::size_t)

        EVENT(is_editable_changed, combo_box&, bool)
        PROP_CONSTEXPR_EVENT(is_editable, bool)

    public:
#define ADD_COMBO_BOX_MEMBERS()  \
    ADD_CONTROL_MEMBERS();       \
    ADD_PROP(items);             \
    ADD_PROP_EVENT(sel_id);      \
    ADD_PROP_EVENT(is_editable); \
    ADD_PROP_EVENT(text);        \
    ADD_DEF_PROP(text)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, combo_box);
            ADD_CTOR_DEF();
            ADD_COMBO_BOX_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_COMBO_BOX_HPP
