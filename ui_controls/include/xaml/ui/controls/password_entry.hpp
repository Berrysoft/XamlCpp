#ifndef XAML_UI_PASSWORD_ENTRY_HPP
#define XAML_UI_PASSWORD_ENTRY_HPP

#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>
#include <xaml/ui/controls/entry.hpp>

namespace xaml
{
    class password_entry;

    template <>
    struct type_guid<password_entry>
    {
        static constexpr guid value{ 0x78f0bbc1, 0x9817, 0x4613, { 0xbd, 0x70, 0xbc, 0x0f, 0x2f, 0x2e, 0x30, 0x64 } };
    };

    class password_entry : public entry
    {
    public:
        META_CLASS_IMPL(entry)

    public:
        XAML_UI_CONTROLS_API password_entry();
        XAML_UI_CONTROLS_API ~password_entry() override;

    public:
        XAML_UI_CONTROLS_API void __draw(rectangle const& region) override;

    private:
        XAML_UI_CONTROLS_API void draw_password_char();

    public:
        EVENT(password_char_changed, std::shared_ptr<password_entry>, char_t)
        PROP_CONSTEXPR_EVENT(password_char, char_t)

    public:
#define ADD_PASSWORD_ENTRY_MEMBERS() \
    ADD_ENTRY_MEMBERS();             \
    ADD_PROP_EVENT(password_char)

        REGISTER_CLASS_DECL(xaml, password_entry, "xaml/ui/controls")
        {
            ADD_CTOR_DEF();
            ADD_PASSWORD_ENTRY_MEMBERS();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml

#endif // !XAML_UI_PASSWORD_ENTRY_HPP
