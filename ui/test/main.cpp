#include <iostream>
#include <xaml/ui/application.hpp>
#include <xaml/ui/button.hpp>
#include <xaml/ui/canvas.hpp>
#include <xaml/ui/grid.hpp>
#include <xaml/ui/meta.hpp>
#include <xaml/ui/msgbox.hpp>
#include <xaml/ui/window.hpp>

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
    btn->add_click([wnd, btn](button const&) { msgbox(wnd, U("Hello world!"), U("Hello"), msgbox_style::info); btn->set_text(U("Hello world!")); });
    g->add_child(btn);
    grid::set_column(btn, 1);
    grid::set_row(btn, 1);
    auto cv = make_shared<canvas>();
    cv->set_margin({ 10, 10, 10, 10 });
    cv->add_redraw([](canvas const& cv, drawing_context& dc) {
        auto cx = cv.get_width() / 2;
        auto cy = cv.get_height() / 2;
        dc.draw_ellipse({ cx - 50, cy - 50, 100, 100 });
    });
    g->add_child(cv);
    grid::set_column(cv, 0);
    grid::set_row(cv, 2);
    wnd->show();
    return app->run();
}
