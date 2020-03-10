#ifndef XAML_UI_CONTROLS_CHECK_BOX_HPP
#define XAML_UI_CONTROLS_CHECK_BOX_HPP

#include <xaml/ui/controls/button.hpp>

namespace xaml
{
    class check_box : public button
    {
    public:
        XAML_UI_CONTROLS_API check_box();
        XAML_UI_CONTROLS_API ~check_box() override;

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

    public:
        PROP_CONSTEXPR_EVENT(is_checked, bool)
        EVENT(is_checked_changed, check_box&, bool)

    public:
#define ADD_CHECK_BOX_MEMBERS() \
    ADD_BUTTON_MEMBERS();       \
    ADD_PROP_EVENT(is_checked)

        REGISTER_CLASS_DECL(xaml, check_box, "xaml/ui/controls")
        {
            ADD_CTOR_DEF();
            ADD_CHECK_BOX_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_CHECK_BOX_HPP
