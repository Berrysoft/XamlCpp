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
        LRESULT wnd_proc(window_message const& msg) override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_GTK3
    private:
        static void on_clicked(GtkButton* button, gpointer data);
#endif // XAML_UI_GTK3

    public:
        void draw(rectangle const& region) override;

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

        PROP(is_default, bool)

        EVENT(click, button const&)
        EVENT(dbclick, button const&)

    public:
        static void register_class() noexcept
        {
            REGISTER_TYPE();
            ADD_CTOR_DEF();
            ADD_PROP(text);
            ADD_PROP(is_default);
        }
    };
} // namespace xaml

#endif // !XAML_UI_BUTTON_HPP
