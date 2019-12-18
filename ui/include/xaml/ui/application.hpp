#ifndef XAML_UI_APPLICATION_HPP
#define XAML_UI_APPLICATION_HPP

#include <memory>
#include <xaml/meta/meta_macro.hpp>

namespace xaml
{
    class application : public meta_class_impl<application>, public std::enable_shared_from_this<application>
    {
    private:
        static constexpr std::string_view class_name = "application";

        friend class window;

        int wnd_num;

#ifdef XAML_UI_GTK3
        void decrease_quit();
#endif

        static std::shared_ptr<application> _current;

    public:
        void init();
        int run();

        static std::shared_ptr<application> current() { return _current; }
    };
} // namespace xaml

#endif // !XAML_UI_APPLICATION_HPP
