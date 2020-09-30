#include <QApplication>
#include <QScreen>
#include <xaml/ui/drawing_conv.hpp>
#include <xaml/ui/monitor.h>

using namespace std;

xaml_result XAML_CALL xaml_monitor_get_all(xaml_vector_view<xaml_monitor>** ptr) noexcept
{
    xaml_ptr<xaml_vector<xaml_monitor>> result;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&result));
    for (QScreen* screen : QApplication::screens())
    {
        xaml_monitor m{ xaml_from_native(screen->geometry()), xaml_from_native(screen->availableGeometry()) };
        XAML_RETURN_IF_FAILED(result->append(move(m)));
    }
    return result->query(ptr);
}
