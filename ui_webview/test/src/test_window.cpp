#include <test_window.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/controls/button.hpp>
#include <xaml/ui/controls/entry.hpp>
#include <xaml/ui/controls/grid.hpp>
#include <xaml/ui/controls/webview.hpp>

using namespace std;

namespace xaml::test
{
    void test_window::init_components()
    {
        set_title(U("Test Window"));
        set_location({ 100, 100 });
        set_size({ 800, 600 });
        auto g = make_shared<grid>();
        g->add_column({ 1, grid_layout::star });
        g->add_column({ 50, grid_layout::abs });
        g->add_row({ 0, grid_layout::compact });
        g->add_row({ 1, grid_layout::star });
        set_child(g);
        auto abar = make_shared<entry>();
        g->add_child(abar);
        auto abutton = make_shared<button>();
        abutton->set_text(U("Go"));
        g->add_child(abutton);
        grid::set_column(*abutton, 1);
        auto view = make_shared<webview>();
        auto argv = application::current()->get_cmd_lines();
        if (argv.size() > 1)
            view->set_uri(to_string_t(argv[1]));
        else
            view->set_uri(U("https://www.bing.com/"));
        g->add_child(view);
        grid::set_row(*view, 1);
        grid::set_column_span(*view, 2);
        abar->set_text(view->get_uri());
        view->add_uri_changed([abar](shared_ptr<webview>, string_view_t uri) { abar->set_text(uri); });
        abutton->add_click([abar, view](shared_ptr<button>) { view->set_uri(abar->get_text()); });
    }
} // namespace xaml::test
