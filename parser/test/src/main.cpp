#include <test.xaml.h>
#include <xaml/meta/meta_context.h>
#include <xaml/parser/parser.h>
#include <xaml/ui/application.h>

using namespace std;

xaml_result XAML_CALL xaml_main(xaml_application* app, int* pcode) noexcept
{
    xaml_ptr<xaml_meta_context> ctx;
    XAML_RETURN_IF_FAILED(xaml_meta_context_new(&ctx));
    XAML_RETURN_IF_FAILED(xaml_parser_register(ctx.get()));
    {
        xaml_ptr<xaml_module> m;
        XAML_RETURN_IF_FAILED(xaml_module_new(&m));
        xaml_ptr<xaml_string> path;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("xaml_ui"), &path));
        XAML_RETURN_IF_FAILED(m->open(path.get()));
        XAML_RETURN_IF_FAILED(ctx->add_module(m.get()));
    }
    {
        xaml_ptr<xaml_module> m;
        XAML_RETURN_IF_FAILED(xaml_module_new(&m));
        xaml_ptr<xaml_string> path;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("xaml_ui_controls"), &path));
        XAML_RETURN_IF_FAILED(m->open(path.get()));
        XAML_RETURN_IF_FAILED(ctx->add_module(m.get()));
    }
    {
        xaml_ptr<xaml_module> m;
        XAML_RETURN_IF_FAILED(xaml_module_new(&m));
        xaml_ptr<xaml_string> path;
        XAML_RETURN_IF_FAILED(xaml_string_new(U("xaml_ui_canvas"), &path));
        XAML_RETURN_IF_FAILED(m->open(path.get()));
        XAML_RETURN_IF_FAILED(ctx->add_module(m.get()));
    }
    XAML_RETURN_IF_FAILED(xaml_test_window_register(ctx.get()));
    xaml_ptr<xaml_test_window> wnd;
    XAML_RETURN_IF_FAILED(xaml_test_window_new(ctx.get(), &wnd));
    XAML_RETURN_IF_FAILED(wnd->show());
    return app->run(pcode);
}
