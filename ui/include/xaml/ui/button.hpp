#ifndef XAML_UI_BUTTON_HPP
#define XAML_UI_BUTTON_HPP

#include <xaml/meta/default_property.hpp>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class button_base : public common_control
    {
    public:
        XAML_API button_base();
        XAML_API virtual ~button_base() override;

#ifdef XAML_UI_WINDOWS
    public:
        XAML_API std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static void on_clicked(GtkButton* button, gpointer data);
#endif // XAML_UI_GTK3

#ifdef XAML_UI_COCOA
    public:
        void __on_action();
#endif // XAML_UI_COCOA

    public:
        XAML_API void __draw(rectangle const& region) override;
        XAML_API void __size_to_fit() override;

    private:
        XAML_API void draw_size();
        XAML_API void draw_text();
        XAML_API void draw_default();

    private:
        string_t m_text{};

    public:
        string_view_t get_text() const { return m_text; }
        void set_text(string_view_t value)
        {
            if (m_text != value)
            {
                m_text = (string_t)value;
                m_text_changed(*this, m_text);
            }
        }

        EVENT(text_changed, button_base&, string_view_t)

        PROP_EVENT(is_default, bool)
        EVENT(is_default_changed, button_base&, bool)

        EVENT(click, button_base&)

    public:
#define ADD_BUTTON_BASE_MEMBERS() \
    ADD_COMMON_CONTROL_MEMBERS(); \
    ADD_PROP_EVENT(text);         \
    ADD_PROP_EVENT(is_default);   \
    ADD_EVENT(click)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, button_base);
            ADD_BUTTON_BASE_MEMBERS();
        }
    };

    class button : public button_base, public meta_class_impl<button>
    {
    public:
        button() : button_base() {}
        ~button() override {}

#define ADD_BUTTON_MEMBERS()   \
    ADD_BUTTON_BASE_MEMBERS(); \
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
