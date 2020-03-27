#ifndef XAML_UI_BUTTON_HPP
#define XAML_UI_BUTTON_HPP

#include <xaml/meta/default_property.hpp>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class button;

    template <>
    struct type_guid<button>
    {
        static constexpr guid value{ 0xe35d0bbe, 0xbec8, 0x4376, { 0xaa, 0xb6, 0xa9, 0x01, 0xed, 0xb4, 0x3d, 0x9f } };
    };

    class button : public control
    {
    public:
        META_CLASS_IMPL(control)

    public:
        XAML_UI_CONTROLS_API button();
        XAML_UI_CONTROLS_API virtual ~button() override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_CONTROLS_API std::optional<std::intptr_t> __wnd_proc(window_message const& msg) override;
        XAML_UI_CONTROLS_API void __size_to_fit() override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    protected:
        static void on_clicked(void* button, void* data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_action();

    protected:
        XAML_UI_CONTROLS_API virtual void draw_size() override;
#endif // XAML_UI_COCOA

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    protected:
        XAML_UI_CONTROLS_API virtual void draw_text();
        XAML_UI_CONTROLS_API virtual void draw_default();

    public:
        EVENT(text_changed, std::shared_ptr<button>, string_view_t)
        PROP_STRING_EVENT(text)

        PROP_EVENT(is_default, bool)
        EVENT(is_default_changed, std::shared_ptr<button>, bool)

        EVENT(click, std::shared_ptr<button>)

    public:
#define ADD_BUTTON_MEMBERS()    \
    ADD_CONTROL_MEMBERS();      \
    ADD_PROP_EVENT(text);       \
    ADD_PROP_EVENT(is_default); \
    ADD_EVENT(click);           \
    ADD_DEF_PROP(text)

        REGISTER_CLASS_DECL(xaml, button, "xaml/ui/controls")
        {
            ADD_CTOR_DEF();
            ADD_BUTTON_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_BUTTON_HPP
