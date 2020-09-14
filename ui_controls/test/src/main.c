#include <test_window.h>
#include <xaml/ui/application.h>

xaml_result XAML_CALL xaml_main(xaml_application* app)
{
    xaml_result hr;
    // Construct a window.
    xaml_test_window* wnd;
    XAML_GOTO_IF_FAILED(xaml_test_window_new(&wnd), exit);
    // Show the window.
    hr = wnd->vtbl->show(wnd);
    wnd->vtbl->release(wnd);
exit:
    return hr;
}
