#include <algorithm>
#include <iostream>
#include <xaml/ui/application.hpp>
#include <xaml/ui/button.hpp>
#include <xaml/ui/canvas.hpp>
#include <xaml/ui/entry.hpp>
#include <xaml/ui/grid.hpp>
#include <xaml/ui/label.hpp>
#include <xaml/ui/msgbox.hpp>
#include <xaml/ui/password_entry.hpp>
#include <xaml/ui/timer.hpp>
#include <xaml/ui/window.hpp>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#endif // XAML_UI_WINDOWS

using namespace std;
using namespace xaml;

// Use different main signature for different platforms.
#ifdef XAML_UI_WINDOWS
INT wWinMain(HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, INT)
#else
int main(int argc, char** argv)
#endif // XAML_UI_WINDOWS
{
    // Initialize and get default application object.
#ifdef XAML_UI_WINDOWS
    auto app = application::init(lpCmdLine);
#else
    auto app = application::init(argc, argv);
#endif
    // Construct a window.
    auto wnd = make_shared<window>();
    // Set title, location, and size.
    // Cover the string constant with `U()` macro.
    wnd->set_title(U("Test Window"));
    wnd->set_location({ 100, 100 });
    wnd->set_size({ 800, 600 });
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
    wnd->set_child(g);
    // Construct a button.
    auto btn = make_shared<button>();
    // Set margin and text.
    btn->set_margin({ 10, 10, 10, 10 });
    btn->set_text(U("Hello"));
    // Construct a timer with interval of 2s.
    auto tmr = make_shared<timer>(2s);
    int count = 0;
    // Add a handler for timer.
    tmr->add_tick([wnd, tmr, &count](timer const&) {
        // Show a same message box for 3 timess.
        msgbox(wnd, U("Hello world!"), U("Hello"), msgbox_style::info);
        if (++count >= 3) tmr->stop();
    });
    // Add a handler for button.
    btn->add_click([btn, tmr, &count](button_base const&) {
        btn->set_text(U("Hello world!"));
        count = 0;
        tmr->start();
    });
    // Set the button to be placed in the center vertically.
    btn->set_valignment(valignment_t::center);
    // Add the button to the grid, and set its column and row.
    g->add_child(btn);
    grid::set_column(btn, 1);
    grid::set_row(btn, 2);
    // Construct a label.
    auto lb = make_shared<label>();
    // Set the margin, text alignmemt, vertical alignment and text.
    lb->set_margin({ 10, 10, 10, 10 });
    lb->set_text_halignment(halignment_t::right);
    lb->set_valignment(valignment_t::center);
    lb->set_text(U("Username:"));
    // Add the label to the grid, and set its column and row.
    g->add_child(lb);
    grid::set_column(lb, 0);
    grid::set_row(lb, 0);
    // Construct an entry (single-line text box).
    auto et = make_shared<entry>();
    et->set_margin({ 0, 5, 0, 5 });
    et->set_text(U("John"));
    et->set_halignment(halignment_t::left);
    et->set_valignment(valignment_t::center);
    g->add_child(et);
    grid::set_column(et, 1);
    grid::set_row(et, 0);
    auto lb2 = make_shared<label>();
    lb2->set_margin({ 10, 10, 10, 10 });
    lb2->set_text_halignment(halignment_t::right);
    lb2->set_valignment(valignment_t::center);
    lb2->set_text(U("Password:"));
    g->add_child(lb2);
    grid::set_column(lb2, 0);
    grid::set_row(lb2, 1);
    auto pet = make_shared<password_entry>();
    pet->set_margin({ 0, 5, 0, 5 });
    pet->set_text(U("123456"));
    pet->set_valignment(valignment_t::center);
    g->add_child(pet);
    grid::set_column(pet, 1);
    grid::set_row(pet, 1);
    // Construct a cnavas.
    auto cv = make_shared<canvas>();
    // Set the margin.
    cv->set_margin({ 10, 10, 10, 10 });
    // Add a handler to the canvas to handle the redraw event.
    // You should always draw in that event.
    cv->add_redraw([](canvas const& cv, drawing_context& dc) {
        // Draw a circle in the center.
        auto cx = cv.get_width() / 2;
        auto cy = cv.get_height() / 2;
        auto r = (min)(cx, cy) - 2;
        dc.draw_ellipse({ colors::black }, { cx - r, cy - r, r * 2, r * 2 });
    });
    // Add the canvas to the grid, and set its column and row.
    g->add_child(cv);
    grid::set_column(cv, 0);
    grid::set_row(cv, 3);
    // Show the window.
    wnd->show();
    // Run the main loop and wait.
    return app->run();
}
