#ifndef XAML_UI_BUTTON_HPP
#define XAML_UI_BUTTON_HPP

#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/control.hpp>

namespace xaml
{
    class button : public control, public meta_class_impl<button>
    {
    private:
        static constexpr std::string_view namespace_name = "xaml";
        static constexpr std::string_view class_name = "button";

    public:
        button();
        virtual ~button() override;

#ifdef XAML_UI_WINDOWS
        void create() override;
#endif // XAML_UI_WINDOWS

        string_t get_text() const;
        void set_text(string_view_t value);

        bool get_is_default() const;
        void set_is_default(bool value);

        static void register_class() noexcept
        {
            REGISTER_TYPE();
            ADD_CTOR_DEF();
            ADD_PROP(text);
            ADD_PROP(location);
            ADD_PROP(x);
            ADD_PROP(y);
            ADD_PROP(size);
            ADD_PROP(width);
            ADD_PROP(height);
            ADD_PROP(is_default);
        }
    };
} // namespace xaml

#endif // !XAML_UI_BUTTON_HPP
