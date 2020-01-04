#include <test_window.hpp>
#include <xaml/deserializer.hpp>
#include <xaml/ui/meta.hpp>

using namespace std;
using namespace xaml;
using namespace xaml::test;

#ifdef XAML_UI_WINDOWS
INT wWinMain(HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, INT)
#else
int main(int argc, char** argv)
#endif // XAML_UI_WINDOWS
{
    init_parser();
    register_class<test_window>();
#ifdef XAML_UI_WINDOWS
    auto app = application::init(lpCmdLine);
#else
    auto app = application::init(argc, argv);
#endif // XAML_UI_WINDOWS
    auto wnd = make_shared<test_window>();
    wnd->init_components();
    wnd->show();
    return app->run();
}
