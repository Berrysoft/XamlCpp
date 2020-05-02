#include <test_window.h>
#include <xaml/ui/application.h>

xaml_result XAML_CALL xaml_main(xaml_application* app)
{
    // Construct a window.
    xaml_test_window* wnd;
    XAML_RETURN_IF_FAILED(xaml_test_window_new(&wnd));
    // Show the window.
    xaml_result hr = wnd->vtbl->show(wnd);
    wnd->vtbl->release(wnd);
    return hr;
}
