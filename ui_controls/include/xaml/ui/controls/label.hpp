#ifndef XAML_UI_LABEL_HPP
#define XAML_UI_LABEL_HPP

#include <atomic>
#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class label : public control
    {
    public:
        XAML_UI_CONTROLS_API label();
        XAML_UI_CONTROLS_API ~label() override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_UI_CONTROLS_API std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
        XAML_UI_CONTROLS_API void __size_to_fit() override;
#endif // XAML_UI_WINDOWS

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    private:
        XAML_UI_CONTROLS_API void draw_size();
        XAML_UI_CONTROLS_API void draw_text();
        XAML_UI_CONTROLS_API void draw_alignment();

        PROP(text_halignment, halignment_t)

        EVENT(text_changed, label&, string_view_t)
        PROP_STRING_EVENT(text)

    public:
#define ADD_LABEL_MEMBERS()    \
    ADD_CONTROL_MEMBERS();     \
    ADD_PROP_EVENT(text);      \
    ADD_PROP(text_halignment); \
    ADD_DEF_PROP(text)

        REGISTER_CLASS_DECL(xaml, label)
        {
            ADD_CTOR_DEF();
            ADD_LABEL_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_LABEL_HPP
