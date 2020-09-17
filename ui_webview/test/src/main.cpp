#include <test_window.h>
#include <xaml/ui/application.h>

xaml_result XAML_CALL xaml_main(xaml_application*) noexcept
{
    xaml_ptr<xaml_test_window> wnd;
    XAML_RETURN_IF_FAILED(xaml_test_window_new(&wnd));
    return wnd->show();
}
