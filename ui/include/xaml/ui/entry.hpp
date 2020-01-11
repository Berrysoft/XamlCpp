#ifndef XAML_UI_ENTRY_HPP
#define XAML_UI_ENTRY_HPP

#include <xaml/meta/default_property.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class entry : public common_control
    {
    public:
        XAML_API entry();
        XAML_API ~entry() override;

    public:
        XAML_API void __draw(rectangle const& region) override;
        XAML_API void __size_to_fit() override;

    private:
        XAML_API void draw_size();
        XAML_API void draw_text();
        XAML_API void draw_alignment();

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

        PROP(text_halignment, halignment_t)

        EVENT(text_changed, entry&, string_view_t)

    public:
#define ADD_ENTRY_MEMBERS()       \
    ADD_COMMON_CONTROL_MEMBERS(); \
    ADD_PROP_EVENT(text);         \
    ADD_PROP(text_halignment);    \
    ADD_DEF_PROP(text)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, entry);
            ADD_CTOR_DEF();
            ADD_ENTRY_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_ENTRY_HPP
