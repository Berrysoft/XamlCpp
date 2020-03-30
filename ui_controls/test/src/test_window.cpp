#include <algorithm>
#include <cmath>
#include <test_window.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/colors.hpp>
#include <xaml/ui/controls/combo_box.hpp>
#include <xaml/ui/controls/entry.hpp>
#include <xaml/ui/controls/grid.hpp>
#include <xaml/ui/controls/label.hpp>
#include <xaml/ui/controls/menu_item.hpp>
#include <xaml/ui/controls/password_entry.hpp>
#include <xaml/ui/controls/progress.hpp>
#include <xaml/ui/controls/stack_panel.hpp>
#include <xaml/ui/filebox.hpp>
#include <xaml/ui/menu_bar.hpp>
#include <xaml/ui/msgbox.hpp>

#if __has_include(<numbers>)
#include <numbers>
using std::numbers::pi;
#else
inline constexpr double pi = M_PI;
#endif // __has_include(<numbers>)

using namespace std;
using namespace xaml;

namespace xaml::test
{
    test_window::test_window() : window(), tmr(2s), combo_source({ U("A"), U("BBB"), U("C") })
    {
        tmr.add_tick(mem_fn_bind(&test_window::on_timer_tick, this));
        add_closing(mem_fn_bind(&test_window::on_window_closing, this));
    }

    void test_window::init_components()
    {
        // Set title, location, and size.
        // Cover the string constant with `U()` macro.
        set_title(U("Test Window"));
        set_location({ 100, 100 });
        set_size({ 800, 600 });
        // Construct a grid.
        auto g = make_shared<grid>();
        // Add 3 columns and 3 rows, with same width or height.
        g->add_column({ 1, grid_layout::star });
        g->add_column({ 1, grid_layout::star });
        g->add_column({ 1, grid_layout::star });
        g->add_row({ 1, grid_layout::compact });
        g->add_row({ 1, grid_layout::compact });
        g->add_row({ 1, grid_layout::star });
        g->add_row({ 1, grid_layout::star });
        // Set g as child of the window.
        set_child(g);
        {
            // Construct a button.
            auto btn = make_shared<button>();
            // Set margin and text.
            btn->set_margin({ 10, 10, 10, 10 });
            btn->set_text(U("Hello"));
            // Add a handler for button.
            btn->add_click(mem_fn_bind(&test_window::on_button_click, this));
            // Set the button to be placed in the center vertically.
            btn->set_valignment(valignment_t::center);
            // Add the button to the grid, and set its column and row.
            g->add_child(btn);
            grid::set_column(*btn, 1);
            grid::set_row(*btn, 2);
        }
        {
            // Construct a label.
            auto lb = make_shared<label>();
            // Set the margin, text alignmemt, vertical alignment and text.
            lb->set_margin({ 10, 10, 10, 10 });
            lb->set_text_halignment(halignment_t::right);
            lb->set_valignment(valignment_t::center);
            lb->set_text(U("Username:"));
            // Add the label to the grid, and set its column and row.
            g->add_child(lb);
            grid::set_column(*lb, 0);
            grid::set_row(*lb, 0);
        }
        {
            // Construct an entry (single-line text box).
            auto et = make_shared<entry>();
            et->set_margin({ 0, 5, 0, 5 });
            et->set_text(U("John"));
            et->set_halignment(halignment_t::left);
            et->set_valignment(valignment_t::center);
            g->add_child(et);
            grid::set_column(*et, 1);
            grid::set_row(*et, 0);
        }
        {
            auto lb2 = make_shared<label>();
            lb2->set_margin({ 10, 10, 10, 10 });
            lb2->set_text_halignment(halignment_t::right);
            lb2->set_valignment(valignment_t::center);
            lb2->set_text(U("Password:"));
            g->add_child(lb2);
            grid::set_column(*lb2, 0);
            grid::set_row(*lb2, 1);
        }
        {
            auto pet = make_shared<password_entry>();
            pet->set_margin({ 0, 5, 0, 5 });
            pet->set_text(U("123456"));
            pet->set_valignment(valignment_t::center);
            g->add_child(pet);
            grid::set_column(*pet, 1);
            grid::set_row(*pet, 1);
        }
        {
            // Construct a cnavas.
            auto cv = make_shared<canvas>();
            // Set the margin.
            cv->set_margin({ 10, 10, 10, 10 });
            // Add a handler to the canvas to handle the redraw event.
            // You should always draw in that event.
            cv->add_redraw(mem_fn_bind(&test_window::on_canvas_redraw, this));
            // Add the canvas to the grid, and set its column and row.
            g->add_child(cv);
            grid::set_column(*cv, 0);
            grid::set_row(*cv, 3);
        }
        {
            auto prog = make_shared<progress>();
            prog->set_margin({ 10, 10, 10, 10 });
            prog->set_valignment(valignment_t::center);
            prog->set_minimum(0);
            prog->set_maximum(100);
            prog->set_value(30);
            g->add_child(prog);
            grid::set_column(*prog, 2);
            grid::set_row(*prog, 2);
        }
        {
            auto box = make_shared<combo_box>();
            box->set_halignment(halignment_t::center);
            box->set_valignment(valignment_t::top);
            box->set_margin({ 10, 10, 10, 10 });
            box->set_items(combo_source);
            box->set_sel_id(1);
            g->add_child(box);
            grid::set_column(*box, 1);
            grid::set_row(*box, 3);

            auto panel = make_shared<stack_panel>();
            panel->set_orientation(orientation::vertical);
            panel->set_margin({ 5, 5, 5, 5 });
            {
                auto btn = make_shared<button>();
                btn->set_margin({ 5, 5, 5, 5 });
                btn->set_text(U("Push"));
                btn->add_click([this](shared_ptr<button>) { combo_source.push_back(U("DDDD")); });
                panel->add_child(btn);
            }
            {
                auto btn = make_shared<button>();
                btn->set_margin({ 5, 5, 5, 5 });
                btn->set_text(U("Pop"));
                btn->add_click([this](shared_ptr<button>) { combo_source.pop_back(); });
                panel->add_child(btn);
            }
            {
                auto btn = make_shared<button>();
                btn->set_margin({ 5, 5, 5, 5 });
                btn->set_text(U("Show"));
                btn->add_click([this, box](shared_ptr<button>) {
                    size_t sel = box->get_sel_id();
                    if (sel >= 0 && sel < combo_source.size())
                        msgbox(shared_from_this<window>(), combo_source[sel].get(), U("Show selected item"));
                });
                panel->add_child(btn);
            }
            g->add_child(panel);
            grid::set_column(*panel, 2);
            grid::set_row(*panel, 3);
        }
        {
            auto mbar = make_shared<menu_bar>();
            {
                auto mfile = make_shared<popup_menu_item>();
                mfile->set_text(U("File"));
                {
                    auto mquit = make_shared<menu_item>();
                    mquit->set_text(U("Close"));
                    mquit->add_click([this](shared_ptr<menu_item>) { close(); });
                    mfile->add_submenu(mquit);
                }
                mbar->add_child(mfile);
            }
            set_menu_bar(mbar);
        }
    }

    void test_window::on_timer_tick(timer&)
    {
        msgbox(shared_from_this<window>(), U("Hello world!"), U("Hello"), msgbox_style::info, msgbox_buttons::ok);
        if (++count >= 3)
        {
            tmr.stop();
            open_filebox openbox{};
            openbox.set_title(U("Open file"));
            openbox.set_filters({ { U("XAML file"), U("*.xaml") } });
            bool res = openbox.show(shared_from_this<window>());
            if (res)
            {
                msgbox(shared_from_this<window>(), openbox.get_result(), U("Open file"));
            }
        }
    }

    void test_window::on_button_click(shared_ptr<button> btn)
    {
        btn->set_text(U("Hello world!"));
        count = 0;
        tmr.start();
    }

    void test_window::on_canvas_redraw(shared_ptr<canvas> cv, shared_ptr<drawing_context> dc)
    {
        auto cx = cv->get_width() / 2;
        auto cy = cv->get_height() / 2;
        auto r = (min)(cx, cy) - 2;
        dc->draw_arc({ colors::black }, { cx - r, cy - r, r * 2, r * 2 }, pi, 2 * pi);
        dc->draw_line({ colors::black }, { cx - r, cy }, { cx + r, cy });
        dc->draw_round_rect({ colors::sky_blue }, { cx - r - 1, cy - r - 1, r * 2 + 2, r * 1.618 + 2 }, { r / 10, r / 10 });
        dc->draw_string({ colors::pink }, { U("Arial"), r / 5, false, false, halignment_t::center, valignment_t::bottom }, { cx, cy }, U("Hello world!"));
    }

    void test_window::on_window_closing(shared_ptr<window> win, shared_ptr<meta_box<bool>> handled)
    {
        handled->get() = msgbox(win, U("Do you want to close this window?"), U("Close window"), msgbox_style::question, msgbox_buttons::yes_no) != msgbox_result::yes;
    }
} // namespace xaml::test
