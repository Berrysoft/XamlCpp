#include <atomic>
#include <xaml/async/action.h>

using namespace std;

xaml_result XAML_CALL xaml_async_action_wait(xaml_async_action* action) noexcept
try
{
    atomic_flag flag{};
    xaml_ptr<xaml_delegate<xaml_async_action, xaml_async_status>> old_handler;
    XAML_RETURN_IF_FAILED(action->get_completed(&old_handler));
    xaml_ptr<xaml_delegate<xaml_async_action, xaml_async_status>> handler;
    XAML_RETURN_IF_FAILED(xaml_delegate_new(
        [&](xaml_ptr<xaml_async_action> const& a, xaml_async_status s) noexcept -> xaml_result {
            xaml_result hr = XAML_S_OK;
            if (old_handler) hr = old_handler->invoke(a, s);
            try
            {
                flag.test_and_set();
                return hr;
            }
            XAML_CATCH_RETURN()
        },
        &handler));
    XAML_RETURN_IF_FAILED(action->set_completed(handler));
    if (!flag.test())
        flag.wait(true);
    return action->get_error();
}
XAML_CATCH_RETURN()
