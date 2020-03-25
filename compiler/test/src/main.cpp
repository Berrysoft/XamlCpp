#include <test.xaml.hpp>
#include <xaml/ui/appmain.hpp>

#ifdef XAML_TEST_GEN_FAKE
#include <xaml/parser.hpp>
#endif // XAML_TEST_GEN_FAKE

using namespace std;
using namespace xaml;
using namespace xaml::test;

#ifdef XAML_TEST_GEN_FAKE
meta_context ctx{};
#endif // XAML_TEST_GEN_FAKE

XAML_APP_MAIN_START()
{
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
}
XAML_APP_MAIN_END()
