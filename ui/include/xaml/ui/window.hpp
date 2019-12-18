#ifndef XAML_UI_WINDOW_HPP
#define XAML_UI_WINDOW_HPP

#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/container.hpp>

namespace xaml
{
    class window : public container, public meta_class_impl<window>
    {
    private:
        static constexpr std::string_view namespace_name = "xaml";
        static constexpr std::string_view class_name = "window";

#ifdef XAML_UI_WINDOWS
        virtual LRESULT wnd_proc(window_message const& msg) override;
#endif

    public:
        window();
        virtual ~window() override;

        void show();

        string_t get_title() const;
        void set_title(string_view_t value);

        static void register_class() noexcept
        {
            REGISTER_TYPE();
            ADD_CTOR_DEF();
        }
    };
} // namespace xaml

#endif // !XAML_UI_WINDOW_HPP
