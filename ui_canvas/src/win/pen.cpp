#include <shared/pen.hpp>
#include <wil/com.h>

using namespace std;

xaml_result xaml_solid_pen_impl::create(ID2D1RenderTarget* target, ID2D1Brush** ptr, FLOAT* pwidth) noexcept
{
    wil::com_ptr_t<ID2D1SolidColorBrush, wil::err_returncode_policy> brush;
    XAML_RETURN_IF_FAILED(target->CreateSolidColorBrush(D2D1::ColorF((uint32_t)m_color), &brush));
    XAML_RETURN_IF_FAILED(brush.copy_to(ptr));
    *pwidth = (FLOAT)m_width;
    return XAML_S_OK;
}
