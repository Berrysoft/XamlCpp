#ifndef XAML_UI_PASSWORD_ENTRY_HPP
#define XAML_UI_PASSWORD_ENTRY_HPP

#include <xaml/ui/entry.hpp>

namespace xaml
{
    class password_entry : public entry
    {
    public:
        XAML_API password_entry();
        XAML_API ~password_entry() override;

    public:
        XAML_API void __draw(rectangle const& region) override;

    private:
        XAML_API void draw_password_char();

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
    ADD_ENTRY_MEMBERS();             \
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
