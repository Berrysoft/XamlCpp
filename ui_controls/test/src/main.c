#include <test_window.h>
#include <xaml/ui/application.h>

xaml_result XAML_CALL xaml_main(xaml_application* app, int* pcode)
{
    xaml_result hr;
    // Construct a window.
    xaml_test_window* wnd;
    XAML_GOTO_IF_FAILED(xaml_test_window_new(&wnd), exit);
    // Show the window.
    XAML_GOTO_IF_FAILED(wnd->vtbl->show(wnd), clean_wnd);
    // Run the application.
    hr = app->vtbl->run(app, pcode);
clean_wnd:
    wnd->vtbl->release(wnd);
exit:
    return hr;
}
