#ifndef XAML_UI_WINDOW_HPP
#define XAML_UI_WINDOW_HPP

#include <xaml/meta/meta_macro.hpp>
#include <xaml/ui/container.hpp>

namespace xaml
{
    class window : public container, private meta_class_impl<window>
    {
    private:
        static constexpr std::string_view class_name = "window";

#ifdef XAML_UI_WINDOWS
        virtual LRESULT wnd_proc(window_message const& msg) override;
#endif

    public:
        window();
        ~window() override;

        void show();
    };
} // namespace xaml

#endif // !XAML_UI_WINDOW_HPP
