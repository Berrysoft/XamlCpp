#include <test_window.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/parser.hpp>
#include <xaml/ui/application.hpp>
#include <xaml/ui/meta.hpp>

#ifdef WIN32
#include <Windows.h>
#endif // WIN32

using namespace std;
using namespace xaml;
using namespace xaml::test;

#ifdef WIN32
INT wWinMain(HINSTANCE, HINSTANCE, LPWSTR lpCmdLine, INT)
#else
int main(int argc, char** argv)
#endif // XAML_UI_WINDOWS
{
    init_parser();
    module m("xaml_ui_controls_meta");
    m.register_meta();
    register_class<test_window>();
#ifdef XAML_UI_WINDOWS
    auto app = application::init(lpCmdLine);
#elif defined(XAML_UI_WINRT) || defined(WIN32)
    auto app = application::init();
#else
    auto app = application::init(argc, argv);
#endif
    auto wnd = make_shared<test_window>();
    wnd->init_components();
    wnd->show();
    return app->run();
}
