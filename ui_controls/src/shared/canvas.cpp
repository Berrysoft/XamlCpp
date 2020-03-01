#include <xaml/ui/controls/canvas.hpp>

using namespace std;

namespace xaml
{
#ifndef XAML_UI_WINDOWS
    canvas::canvas() : control()
    {
    }

    canvas::~canvas() {}
#endif // !XAML_UI_WINDOWS

    drawing_context::drawing_context(native_handle_type handle) : m_handle(handle)
    {
    }
} // namespace xaml
