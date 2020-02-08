#ifndef XAML_UI_BUTTON_HPP
#define XAML_UI_BUTTON_HPP

#include <xaml/meta/default_property.hpp>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class button : public common_control
    {
    public:
        XAML_UI_CONTROLS_API button();
        XAML_UI_CONTROLS_API virtual ~button() override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_CONTROLS_API std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    protected:
        static void on_clicked(GtkButton* button, gpointer data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_action();
#endif // XAML_UI_COCOA

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;
        XAML_UI_CONTROLS_API void __size_to_fit() override;

    protected:
        XAML_UI_CONTROLS_API void draw_size();
        XAML_UI_CONTROLS_API void draw_text();
        XAML_UI_CONTROLS_API void draw_default();

    public:
        EVENT(text_changed, button&, string_view_t)
        PROP_STRING_EVENT(text)

        PROP_EVENT(is_default, bool)
        EVENT(is_default_changed, button&, bool)

        EVENT(click, button&)

    public:
#define ADD_BUTTON_MEMBERS()      \
    ADD_COMMON_CONTROL_MEMBERS(); \
    ADD_PROP_EVENT(text);         \
    ADD_PROP_EVENT(is_default);   \
    ADD_EVENT(click);             \
    ADD_DEF_PROP(text)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, button);
            ADD_CTOR_DEF();
            ADD_BUTTON_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_BUTTON_HPP
