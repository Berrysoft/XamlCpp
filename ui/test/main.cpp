#include <algorithm>
#include <iostream>
#include <xaml/ui/meta.hpp>
#include <xaml/ui/msgbox.hpp>
#include <xaml/ui/timer.hpp>

using namespace std;
using namespace xaml;

#ifdef XAML_UI_WINDOWS
INT wWinMain(HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, INT)
#else
int main(int argc, char** argv)
#endif // XAML_UI_WINDOWS
{
    init_meta();

#ifdef XAML_UI_WINDOWS
    auto app = application::init(lpCmdLine);
#else
    auto app = application::init(argc, argv);
#endif // XAML_UI_WINDOWS
    auto wnd = make_shared<window>();
    wnd->set_title(U("Test Window"));
    wnd->set_location({ 100, 100 });
    wnd->set_size({ 800, 600 });
    auto g = make_shared<grid>();
    g->add_column({ 1, grid_layout::star });
    g->add_column({ 1, grid_layout::star });
    g->add_column({ 1, grid_layout::star });
    g->add_row({ 1, grid_layout::star });
    g->add_row({ 1, grid_layout::star });
    g->add_row({ 1, grid_layout::star });
    wnd->set_child(g);
    auto btn = static_pointer_cast<button>(construct(*get_type("xaml", "button")));
    btn->set_margin({ 10, 10, 10, 10 });
    btn->set_size({ 150, 50 });
    btn->set_text(U("Hello"));
    auto tmr = make_shared<timer>(chrono::milliseconds(2000));
    int count = 0;
    tmr->add_tick([wnd, tmr, &count](timer const&) {
        msgbox(wnd, U("Hello world!"), U("Hello"), msgbox_style::info);
        if (++count >= 3) tmr->stop();
    });
    btn->add_click([btn, tmr](button const&) {
        btn->set_text(U("Hello world!"));
        tmr->start();
    });
    btn->set_valignment(valignment_t::center);
    g->add_child(btn);
    grid::set_column(btn, 1);
    grid::set_row(btn, 1);
    auto lb = make_shared<label>();
    lb->set_margin({ 10, 10, 10, 10 });
    lb->set_text_halignment(halignment_t::right);
    lb->set_valignment(valignment_t::center);
    lb->set_text(U("This is a label."));
    g->add_child(lb);
    grid::set_column(lb, 0);
    grid::set_row(lb, 1);
    auto cv = make_shared<canvas>();
    cv->set_margin({ 10, 10, 10, 10 });
    cv->add_redraw([](canvas const& cv, drawing_context& dc) {
        auto cx = cv.get_width() / 2;
        auto cy = cv.get_height() / 2;
        auto r = (min)(cx, cy) - 2;
        dc.draw_ellipse({ colors::black }, { cx - r, cy - r, r * 2, r * 2 });
    });
    g->add_child(cv);
    grid::set_column(cv, 0);
    grid::set_row(cv, 2);
    wnd->show();
    return app->run();
}
