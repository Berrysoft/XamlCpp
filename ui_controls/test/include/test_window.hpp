#ifndef XAML_UI_TEST_TEST_WINDOW_HPP
#define XAML_UI_TEST_TEST_WINDOW_HPP

#include <xaml/observable_vector.hpp>
#include <xaml/ui/controls/button.hpp>
#include <xaml/ui/controls/canvas.hpp>
#include <xaml/ui/timer.hpp>
#include <xaml/ui/window.hpp>

namespace xaml::test
{
    class test_window : public window
    {
    private:
        timer tmr;
        int count{ 0 };
        observable_vector<string_t> combo_source;

    public:
        test_window();

        ~test_window() override {}

        void init_components();

        void on_timer_tick(timer&);

        void on_button_click(std::shared_ptr<button> btn);

        void on_canvas_redraw(std::shared_ptr<canvas> cv, std::shared_ptr<drawing_context> dc);
    };
} // namespace xaml::test

#endif // !XAML_UI_TEST_TEST_WINDOW_HPP
