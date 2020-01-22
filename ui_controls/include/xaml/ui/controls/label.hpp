#ifndef XAML_UI_LABEL_HPP
#define XAML_UI_LABEL_HPP

#include <atomic>
#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class label : public common_control
    {
    public:
        XAML_API label();
        XAML_API ~label() override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_API std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

    public:
        XAML_API void __draw(rectangle const& region) override;
        XAML_API void __size_to_fit() override;

    private:
        XAML_API void draw_size();
        XAML_API void draw_text();
        XAML_API void draw_alignment();

        PROP(text_halignment, halignment_t)

        EVENT(text_changed, label&, string_view_t)
        PROP_STRING_EVENT(text)

    public:
#define ADD_LABEL_MEMBERS()       \
    ADD_COMMON_CONTROL_MEMBERS(); \
    ADD_PROP_EVENT(text);         \
    ADD_PROP(text_halignment);    \
    ADD_DEF_PROP(text)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, label);
            ADD_CTOR_DEF();
            ADD_LABEL_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_LABEL_HPP
