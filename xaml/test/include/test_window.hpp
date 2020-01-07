#ifndef XAML_TEST_TEST_WINDOW_HPP
#define XAML_TEST_TEST_WINDOW_HPP

#include <xaml/ui/button.hpp>
#include <xaml/ui/canvas.hpp>
#include <xaml/ui/timer.hpp>
#include <xaml/ui/window.hpp>

namespace xaml::test
{
    class test_window : public window, public meta_class_impl<test_window>
    {
    private:
        timer tmr;
        int count{ 0 };

    public:
        test_window();

        ~test_window() override {}

        void init_components();

        void on_timer_tick(timer&);

        void on_button_click(button_base& btn);

        void on_canvas_redraw(canvas& cv, drawing_context& dc);

        static void register_class() noexcept
        {
            REGISTER_TYPE(xaml::test, test_window);
            ADD_CTOR_DEF();
            ADD_WINDOW_MEMBERS();
            ADD_METHOD(on_button_click);
            ADD_METHOD(on_canvas_redraw);
        }
    };
} // namespace xaml::test

#endif // !XAML_TEST_TEST_WINDOW_HPP
