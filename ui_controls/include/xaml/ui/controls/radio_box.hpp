#ifndef XAML_UI_CONTROLS_RADIO_BOX_HPP
#define XAML_UI_CONTROLS_RADIO_BOX_HPP

#include <xaml/ui/controls/button.hpp>

namespace xaml
{
    class radio_box : public button
    {
    public:
        XAML_UI_CONTROLS_API radio_box();
        XAML_UI_CONTROLS_API ~radio_box() override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_CONTROLS_API std::optional<std::intptr_t> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    protected:
        static void on_toggled(void* button, void* data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_state_changed();
#endif // XAML_UI_COCOA

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

#ifdef XAML_UI_WINDOWS
        XAML_UI_CONTROLS_API void __size_to_fit() override;
#endif // XAML_UI_WINDOWS

    protected:
        XAML_UI_CONTROLS_API virtual void draw_checked();
        XAML_UI_CONTROLS_API virtual void draw_group();

    public:
        PROP_CONSTEXPR_EVENT(is_checked, bool)
        EVENT(is_checked_changed, std::reference_wrapper<radio_box>, bool)

        PROP_STRING(group)

    public:
#define ADD_RADIO_BOX_MEMBERS() \
    ADD_BUTTON_MEMBERS();       \
    ADD_PROP_EVENT(is_checked); \
    ADD_PROP(group)

        REGISTER_CLASS_DECL(xaml, radio_box, "xaml/ui/controls")
        {
            ADD_CTOR_DEF();
            ADD_RADIO_BOX_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_RADIO_BOX_HPP
