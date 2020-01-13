#ifndef XAML_UI_PASSWORD_ENTRY_HPP
#define XAML_UI_PASSWORD_ENTRY_HPP

#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class password_entry : public common_control
    {
    public:
        XAML_API password_entry();
        XAML_API ~password_entry() override;

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
        XAML_API void draw_password_char();

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

        EVENT(text_changed, password_entry&, string_view_t)

    private:
        char_t m_password_char{};

    public:
        constexpr char_t get_password_char() const noexcept { return m_password_char; }
        void set_password_char(char_t value) noexcept
        {
            if (m_password_char != value)
            {
                m_password_char = value;
                m_password_char_changed(*this, m_password_char);
            }
        }

        EVENT(password_char_changed, password_entry&, char_t)

    public:
#define ADD_PASSWORD_ENTRY_MEMBERS() \
    ADD_COMMON_CONTROL_MEMBERS();    \
    ADD_PROP_EVENT(text);            \
    ADD_DEF_PROP(text);              \
    ADD_PROP_EVENT(password_char)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, password_entry);
            ADD_CTOR_DEF();
            ADD_PASSWORD_ENTRY_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_PASSWORD_ENTRY_HPP
