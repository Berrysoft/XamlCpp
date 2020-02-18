#include <test_window.hpp>
#include <xaml/ui/controls/grid.hpp>
#include <xaml/ui/webview/webview.hpp>

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
        view->set_uri(U("https://www.bing.com/"));
        g->add_child(view);
    }
} // namespace xaml::test
