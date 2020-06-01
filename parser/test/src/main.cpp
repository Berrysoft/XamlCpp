#include <test.xaml.h>
#include <test_converter.h>
#include <xaml/meta/meta_context.h>
#include <xaml/parser/parser.h>
#include <xaml/ui/application.h>

using namespace std;

xaml_result XAML_CALL xaml_main(xaml_application* app, int* pcode) noexcept
{
    xaml_ptr<xaml_meta_context> ctx;
    XAML_RETURN_IF_FAILED(xaml_meta_context_new(&ctx));
    XAML_RETURN_IF_FAILED(ctx->add_module_recursive(U("xaml_markup")));
    XAML_RETURN_IF_FAILED(ctx->add_module_recursive(U("xaml_ui_controls")));
    XAML_RETURN_IF_FAILED(ctx->add_module_recursive(U("xaml_ui_canvas")));
    XAML_RETURN_IF_FAILED(xaml_test_window_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_test_model_register(ctx));
    XAML_RETURN_IF_FAILED(xaml_test_converter_register(ctx));
    xaml_ptr<xaml_test_window> wnd;
    XAML_RETURN_IF_FAILED(xaml_test_window_new(ctx, &wnd));
    XAML_RETURN_IF_FAILED(wnd->show());
    return app->run(pcode);
}
