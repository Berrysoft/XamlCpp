#include <test_window.hpp>
#include <xaml/ui/appmain.hpp>

using namespace std;
using namespace xaml;
using namespace xaml::test;

XAML_APP_MAIN_START()
{
    // Construct a window.
    auto wnd = make_shared<test_window>();
    // Initialize the window.
    wnd->init_components();
    // Show the window.
    wnd->show();
}
XAML_APP_MAIN_END()
