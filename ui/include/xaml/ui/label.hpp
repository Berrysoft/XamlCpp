#ifndef XAML_UI_LABEL_HPP
#define XAML_UI_LABEL_HPP

#include <atomic>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class label : public common_control, public meta_class_impl<label>
    {
    private:
        static constexpr std::string_view namespace_name = "xaml";
        static constexpr std::string_view class_name = "label";

    public:
        label();
        ~label() override;

#ifdef XAML_UI_WINDOWS
    protected:
        size __get_compact_size() const override;
#endif // XAML_UI_WINDOWS

#ifdef XAML_UI_COCOA
    public:
        void __size_to_fit() override;
#endif // XAML_UI_COCOA

    public:
        void __draw(rectangle const& region) override;

    private:
        void draw_size();
        void draw_text();
        void draw_alignment();

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

        EVENT(text_changed, label const&, string_view_t)

    public:
#define ADD_LABEL_MEMBERS()       \
    ADD_COMMON_CONTROL_MEMBERS(); \
    ADD_PROP_EVENT(text)

        static void register_class() noexcept
        {
            REGISTER_TYPE();
            ADD_CTOR_DEF();
            ADD_LABEL_MEMBERS();
        }
    };
} // namespace xaml

#endif // !XAML_UI_LABEL_HPP
