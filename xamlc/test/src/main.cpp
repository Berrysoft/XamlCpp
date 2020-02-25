#include <test_window.hpp>
#include <xaml/ui/application.hpp>

#ifdef XAML_TEST_GEN_FAKE
#include <xaml/parser.hpp>
#endif // XAML_TEST_GEN_FAKE

#if defined(WIN32) || defined(__MINGW32__)
#include <Windows.h>
#include <tchar.h>
#endif // WIN32 || __MINGW32__

using namespace std;
using namespace xaml;
using namespace xaml::test;

#if defined(WIN32) || defined(__MINGW32__)
INT APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR lpCmdLine, INT)
#else
int main(int argc, char** argv)
#endif // WIN32 || __MINGW32__
{
#if defined(WIN32) || defined(__MINGW32__)
    auto app = application::init(lpCmdLine);
#else
    auto app = application::init(argc, argv);
#endif // WIN32 || __MINGW32__

#ifdef XAML_TEST_GEN_FAKE
    init_parser();
    register_class<test_window>();
    app->add_module("xaml_ui_meta");
    app->add_module("xaml_ui_controls_meta");
#endif // XAML_TEST_GEN_FAKE

    auto wnd = make_shared<test_window>();
    wnd->init_components();
    wnd->show();
    return app->run();
}
