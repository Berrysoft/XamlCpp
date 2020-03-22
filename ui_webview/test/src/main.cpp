#include <test_window.hpp>
#include <xaml/ui/appmain.hpp>

using namespace std;
using namespace xaml;
using namespace xaml::test;

XAML_APP_MAIN_START()
{
    auto wnd = make_shared<test_window>();
    wnd->init_components();
    wnd->show();
}
XAML_APP_MAIN_END()
