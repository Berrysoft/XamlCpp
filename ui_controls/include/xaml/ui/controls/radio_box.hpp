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
        XAML_UI_CONTROLS_API std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

#ifdef XAML_UI_WINDOWS
        XAML_UI_CONTROLS_API void __size_to_fit() override;
#endif // XAML_UI_WINDOWS

    private:
        void draw_checked();

    public:
        PROP_CONSTEXPR_EVENT(is_checked, bool)
        EVENT(is_checked_changed, radio_box&, bool)

        PROP_STRING(group)

    public:
#define ADD_RADIO_BOX_MEMBERS() \
    ADD_BUTTON_MEMBERS();       \
    ADD_PROP_EVENT(is_checked); \
    ADD_PROP(group)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, radio_box);
            ADD_CTOR_DEF();
            ADD_RADIO_BOX_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_CONTROLS_RADIO_BOX_HPP
