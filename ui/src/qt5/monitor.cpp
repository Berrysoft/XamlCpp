#include <QApplication>
#include <QScreen>
#include <xaml/ui/drawing_conv.hpp>
#include <xaml/ui/monitor.h>

xaml_result XAML_CALL xaml_monitor_get_all(xaml_vector_view** ptr) noexcept
{
    xaml_ptr<xaml_vector> result;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&result));
    for (QScreen* screen : QApplication::screens())
    {
        xaml_monitor m{ xaml_from_native(screen->geometry()), xaml_from_native(screen->availableGeometry()) };
        xaml_ptr<xaml_object> box;
        XAML_RETURN_IF_FAILED(xaml_box_value(m, &box));
        XAML_RETURN_IF_FAILED(result->append(box));
    }
    return result->query(ptr);
}
