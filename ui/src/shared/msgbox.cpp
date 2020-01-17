#include <xaml/ui/msgbox.hpp>

using namespace std;

#ifndef USE_MSGBOX_ASYNC
namespace xaml
{
    void msgbox_async(function<void(msgbox_result)> callback, shared_ptr<window> parent, string_view_t message, string_view_t title, msgbox_style style, msgbox_buttons buttons)
    {
        auto res = msgbox(parent, message, title, style, buttons);
        if (callback) callback(res);
    }
} // namespace xaml
#endif // !USE_MSGBOX_ASYNC
