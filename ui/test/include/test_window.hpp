#ifndef XAML_UI_TEST_TEST_WINDOW_HPP
#define XAML_UI_TEST_TEST_WINDOW_HPP

#include <xaml/ui/button.hpp>
#include <xaml/ui/canvas.hpp>
#include <xaml/ui/timer.hpp>
#include <xaml/ui/window.hpp>

namespace xaml::test
{
    class test_window : public window
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
    };
} // namespace xaml::test

#endif // !XAML_UI_TEST_TEST_WINDOW_HPP
