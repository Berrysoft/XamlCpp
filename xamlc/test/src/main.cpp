#include <test.xaml.hpp>
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

#ifdef XAML_TEST_GEN_FAKE
meta_context ctx{};
#endif // XAML_TEST_GEN_FAKE

#if defined(WIN32) || defined(__MINGW32__)
INT APIENTRY _tWinMain(HINSTANCE, HINSTANCE, LPTSTR lpCmdLine, INT)
#else
int main(int argc, char const* const* argv)
#endif // WIN32 || __MINGW32__
{
#if defined(WIN32) || defined(__MINGW32__)
    auto app = application::init();
#else
    auto app = application::init(argc, argv);
#endif // WIN32 || __MINGW32__
    auto wnd = make_shared<test_window>();

#ifdef XAML_TEST_GEN_FAKE
    init_parser(ctx);
    register_class<test_window>(ctx);
    ctx.add_module(P("xaml_ui_meta"));
    ctx.add_module(P("xaml_ui_controls_meta"));
    ctx.add_module(P("xaml_ui_canvas_meta"));
    wnd->init_components(ctx);
#else
    wnd->init_components();
#endif // XAML_TEST_GEN_FAKE

    wnd->show();
    return app->run();
}
