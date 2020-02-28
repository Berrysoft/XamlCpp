#include <test_window.hpp>
#include <xaml/ui/application.hpp>
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
        set_child(g);
        auto view = make_shared<webview>();
        auto argv = application::current()->get_cmd_lines();
        if (argv.size() > 1)
            view->set_uri(argv[1]);
        else
            view->set_uri(U("https://www.bing.com/"));
        g->add_child(view);
    }
} // namespace xaml::test
