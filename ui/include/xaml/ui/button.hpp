#ifndef XAML_UI_BUTTON_HPP
#define XAML_UI_BUTTON_HPP

#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class button : public common_control, public meta_class_impl<button>
    {
    private:
        static constexpr std::string_view namespace_name = "xaml";
        static constexpr std::string_view class_name = "button";

    public:
        button();
        virtual ~button() override;

#ifdef XAML_UI_WINDOWS
    public:
        std::optional<LRESULT> __wnd_proc(window_message const& msg) override;
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
        void __draw(rectangle const& region) override;
        void __size_to_fit() override;

    private:
        void draw_size();
        void draw_text();
        void draw_default();

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

        EVENT(text_changed, button const&, string_view_t)

        PROP_EVENT(is_default, bool)

        EVENT(click, button const&)

    public:
#define ADD_BUTTON_MEMBERS()      \
    ADD_COMMON_CONTROL_MEMBERS(); \
    ADD_PROP_EVENT(text);         \
    ADD_PROP_EVENT(is_default);   \
    ADD_EVENT(click)

        static void register_class() noexcept
        {
            REGISTER_TYPE();
            ADD_CTOR_DEF();
            ADD_BUTTON_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_BUTTON_HPP
