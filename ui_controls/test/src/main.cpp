#include <test_window.h>
#include <xaml/ui/application.h>

xaml_result XAML_CALL xaml_main(xaml_application* app) noexcept
{
    // Construct a window.
    xaml_ptr<xaml_test_window> wnd;
    XAML_RETURN_IF_FAILED(xaml_test_window_new(&wnd));
    // Show the window.
    XAML_RETURN_IF_FAILED(wnd->show());
    int code;
    return app->run(&code);
}
