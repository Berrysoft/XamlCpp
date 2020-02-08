#ifndef XAML_UI_ENTRY_HPP
#define XAML_UI_ENTRY_HPP

#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class entry : public common_control
    {
    public:
        XAML_UI_CONTROLS_API entry();
        XAML_UI_CONTROLS_API ~entry() override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_CONTROLS_API std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static void on_changed(GtkEditable* editable, gpointer data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_changed();
#endif

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;
        XAML_UI_CONTROLS_API void __size_to_fit() override;

    private:
        XAML_UI_CONTROLS_API void draw_size();
        XAML_UI_CONTROLS_API void draw_text();
        XAML_UI_CONTROLS_API void draw_alignment();

        PROP(text_halignment, halignment_t)

        EVENT(text_changed, entry&, string_view_t)
        PROP_STRING_EVENT(text)

    public:
#define ADD_ENTRY_MEMBERS()       \
    ADD_COMMON_CONTROL_MEMBERS(); \
    ADD_PROP_EVENT(text);         \
    ADD_PROP(text_halignment);    \
    ADD_DEF_PROP(text)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, entry);
            ADD_CTOR_DEF();
            ADD_ENTRY_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_ENTRY_HPP