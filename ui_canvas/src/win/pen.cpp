#include <shared/pen.hpp>
#include <wil/com.h>
#include <xaml/ui/win/controls/brush.h>

using namespace std;

xaml_result xaml_brush_pen_impl::create(ID2D1RenderTarget* target, xaml_rectangle const& region, ID2D1Brush** ptr, FLOAT* pwidth) noexcept
{
    xaml_ptr<xaml_win32_brush> native_brush;
    XAML_RETURN_IF_FAILED(m_brush->query(&native_brush));
    XAML_RETURN_IF_FAILED(native_brush->create(target, region, ptr));
    *pwidth = (FLOAT)m_width;
    return XAML_S_OK;
}
