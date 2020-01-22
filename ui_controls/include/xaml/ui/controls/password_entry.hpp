#ifndef XAML_UI_PASSWORD_ENTRY_HPP
#define XAML_UI_PASSWORD_ENTRY_HPP

#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>
#include <xaml/ui/controls/entry.hpp>

#if defined(XAML_UI_WINDOWS) || defined(XAML_UI_GTK3) || defined(XAML_UI_COCOA)
#define PASSWORD_ENTRY_INHERITS_ENTRY
#endif

namespace xaml
{
#ifdef PASSWORD_ENTRY_INHERITS_ENTRY
    class password_entry : public entry
#else
    class password_entry : public common_control
#endif // PASSWORD_ENTRY_INHERITS_ENTRY
    {
    public:
        XAML_API password_entry();
        XAML_API ~password_entry() override;

    public:
        XAML_API void __draw(rectangle const& region) override;

#ifndef PASSWORD_ENTRY_INHERITS_ENTRY
    public:
        XAML_API void __size_to_fit() override;

    private:
        XAML_API void draw_size();
        XAML_API void draw_text();
#endif // !PASSWORD_ENTRY_INHERITS_ENTRY

    private:
        XAML_API void draw_password_char();

    public:
#ifndef PASSWORD_ENTRY_INHERITS_ENTRY
        EVENT(text_changed, password_entry&, string_view_t)
        PROP_STRING_EVENT(text)
#endif // ! PASSWORD_ENTRY_INHERITS_ENTRY

        EVENT(password_char_changed, password_entry&, char_t)
        PROP_CONSTEXPR_EVENT(password_char, char_t)

    public:
#ifdef PASSWORD_ENTRY_INHERITS_ENTRY
#define ADD_PASSWORD_ENTRY_MEMBERS() \
    ADD_ENTRY_MEMBERS();             \
    ADD_PROP_EVENT(password_char)
#else
#define ADD_PASSWORD_ENTRY_MEMBERS() \
    ADD_COMMON_CONTROL_MEMBERS();    \
    ADD_PROP_EVENT(text);            \
    ADD_DEF_PROP(text);              \
    ADD_PROP_EVENT(password_char)
#endif //  PASSWORD_ENTRY_INHERITS_ENTRY

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, password_entry);
            ADD_CTOR_DEF();
            ADD_PASSWORD_ENTRY_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_PASSWORD_ENTRY_HPP
