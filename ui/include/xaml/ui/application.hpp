#ifndef XAML_UI_APPLICATION_HPP
#define XAML_UI_APPLICATION_HPP

#include <memory>
#include <xaml/meta/meta_macro.hpp>

namespace xaml
{
    class application : public meta_class_impl<application>
    {
    private:
        int wnd_num;

    public:
        int run();
    };
} // namespace xaml

#endif // !XAML_UI_APPLICATION_HPP
