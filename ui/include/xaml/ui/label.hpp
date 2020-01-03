#ifndef XAML_UI_LABEL_HPP
#define XAML_UI_LABEL_HPP

#include <atomic>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class label : public common_control, public meta_class_impl<label>
    {
    public:
        XAML_API label();
        XAML_API ~label() override;

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

        EVENT(text_changed, label&, string_view_t)

    public:
#define ADD_LABEL_MEMBERS()       \
    ADD_COMMON_CONTROL_MEMBERS(); \
    ADD_PROP_EVENT(text);         \
    ADD_PROP(text_halignment)

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml, label);
            ADD_CTOR_DEF();
            ADD_LABEL_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_LABEL_HPP
