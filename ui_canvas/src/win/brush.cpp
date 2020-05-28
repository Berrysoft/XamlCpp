#include <shared/brush.hpp>
#include <wil/com.h>

using namespace std;

xaml_result xaml_solid_brush_impl::create(ID2D1RenderTarget* target, ID2D1Brush** ptr) noexcept
{
    wil::com_ptr_t<ID2D1SolidColorBrush, wil::err_returncode_policy> brush;
    XAML_RETURN_IF_FAILED(target->CreateSolidColorBrush(D2D1::ColorF((uint32_t)m_color), &brush));
    return brush.copy_to(ptr);
}
