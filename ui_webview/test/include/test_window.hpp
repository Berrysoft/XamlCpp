#ifndef XAML_UI_WEBVIEW_TEST_WINDOW_HPP
#define XAML_UI_WEBVIEW_TEST_WINDOW_HPP

#include <xaml/ui/window.hpp>

namespace xaml::test
{
    class test_window : public window
    {
    public:
        test_window() {}

        ~test_window() override {}

        void init_components();
    };
} // namespace xaml::test

#endif // !XAML_UI_WEBVIEW_TEST_WINDOW_HPP
