#include <test_window.h>
#include <xaml/ui/application.h>

xaml_result XAML_CALL xaml_main(xaml_application* app, int* pcode)
{
    xaml_result hr;
    // Construct a window.
    xaml_test_window* wnd;
    hr = xaml_test_window_new(&wnd);
    if (XAML_FAILED(hr)) goto exit;
    // Show the window.
    hr = wnd->vtbl->show(wnd);
    if (XAML_FAILED(hr)) goto exit_clean_wnd;
    hr = app->vtbl->run(app, pcode);
exit_clean_wnd:
    wnd->vtbl->release(wnd);
exit:
    return hr;
}
