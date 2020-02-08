#include <cmath>
#include <test_window.hpp>
#include <xaml/deserializer.hpp>
#include <xaml/parser.hpp>
#include <xaml/ui/msgbox.hpp>

using namespace std;
using namespace xaml;

namespace xaml::test
{
    test_window::test_window() : window(), tmr(2s)
    {
        tmr.add_tick(mem_fn_bind(&test_window::on_timer_tick, this));
    }

    void test_window::init_components()
    {
        parser p("test.xaml");
        if (p.is_open())
        {
            deserializer des{};
            xaml_node node = p.parse();
            des.deserialize(node, static_pointer_cast<test_window>(shared_from_this()));
        }
    }

    void test_window::on_timer_tick(timer&)
    {
        msgbox(static_pointer_cast<window>(shared_from_this()), U("Hello world!"), U("Hello"), msgbox_style::info);
        if (++count) tmr.stop();
    }

    void test_window::on_button_click(button& btn)
    {
        btn.set_text(U("Hello world!"));
        count = 0;
        tmr.start();
    }

    void test_window::on_canvas_redraw(canvas& cv, drawing_context& dc)
    {
        auto cx = cv.get_width() / 2;
        auto cy = cv.get_height() / 2;
        auto r = (min)(cx, cy) - 2;
        dc.draw_ellipse({ colors::black }, { cx - r, cy - r, r * 2, r * 2 });
        point pc{ cx, cy };
        point p1 = point{ 0, -r } + pc;
        point p2 = point{ r * sqrt(3) / 2, r / 2 } + pc;
        point p3 = point{ -r * sqrt(3) / 2, r / 2 } + pc;
        dc.draw_line({ colors::sky_blue }, p1, p2);
        dc.draw_line({ colors::sky_blue }, p2, p3);
        dc.draw_line({ colors::sky_blue }, p3, p1);
    }
} // namespace xaml::test
