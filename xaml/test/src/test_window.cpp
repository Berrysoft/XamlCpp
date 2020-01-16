#include <filesystem>
#include <test_window.hpp>
#include <xaml/deserializer.hpp>
#include <xaml/parser.hpp>
#include <xaml/ui/msgbox.hpp>

using namespace std;
using namespace std::filesystem;
using namespace xaml;

namespace xaml::test
{
    test_window::test_window() : window(), tmr(2s)
    {
        tmr.add_tick(mem_fn_bind(&test_window::on_timer_tick, this));
    }

    void test_window::init_components()
    {
#ifdef XAML_UI_WINRT
        constexpr string_view x = R"(<window x:class="xaml::test::test_window"
        xmlns="https://github.com/Berrysoft/XamlCpp/"
        xmlns:x="https://github.com/Berrysoft/XamlCpp/xaml/"
        title="Test Window" location="200, 100" size="800, 600">
  <grid margin="10" columns="1*, 0.8*, 1*" rows="auto, auto, 0.5*, 1*">
    <label margin="10" text_halignment="right" valignment="center">Username:</label>
    <entry grid.column="1" margin="0, 5" valignment="center">John</entry>
    <label grid.row="1" margin="10" text_halignment="right" valignment="center">Password:</label>
    <password_entry grid.column="1" grid.row="1" margin="0, 5" valignment="center">123456</password_entry>
    <label x:name="mylabel" grid.column="0" grid.row="2" margin="5, 0" text_halignment="center" valignment="center" text="{binding text,element=mybutton,mode=one_way}"/>
    <button x:name="mybutton" grid.column="1" grid.row="2" is_default="true" valignment="center" click="on_button_click">Hello</button>
    <canvas grid.column="0" grid.row="3" margin="10" redraw="on_canvas_redraw"/>
  </grid>
</window>
)";
        parser p(x, parser::load_memory);
#else
        parser p(absolute("test.xaml").string());
#endif // XAML_UI_WINRT
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

    void test_window::on_button_click(button_base& btn)
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
    }
} // namespace xaml::test
