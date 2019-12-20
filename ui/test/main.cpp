#include <iostream>
#include <xaml/ui/application.hpp>
#include <xaml/ui/button.hpp>
#include <xaml/ui/grid.hpp>
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
#ifdef XAML_UI_WINDOWS
    auto app = make_shared<application>(lpCmdLine);
#else
    auto app = make_shared<application>(argc, argv);
#endif // XAML_UI_WINDOWS
    app->init();
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
    auto btn = make_shared<button>();
    btn->set_margin({ 10, 10, 10, 10 });
    btn->set_size({ 150, 50 });
    btn->set_text(U("Hello"));
    btn->add_click([wnd, btn](button const&) { msgbox(wnd, U("Hello world!"), U("Hello"), msgbox_style::info); btn->set_text(U("Hello world!")); });
    g->add_child(btn);
    g->set_column(btn, 1);
    g->set_row(btn, 1);
    wnd->show();
    return app->run();
}
