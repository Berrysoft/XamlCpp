#include "winrt/Microsoft.Graphics.Canvas.UI.Xaml.h"
#include <winrt/drawing.hpp>
#include <xaml/ui/canvas.hpp>

using namespace winrt;
using namespace Windows::UI;
using namespace Microsoft::Graphics::Canvas::UI::Xaml;

namespace xaml
{
    XAML_API drawing_context::drawing_context(native_handle_type handle) : m_handle(handle)
    {
    }

    static Color get_Color(color c) { return ColorHelper::FromArgb(c.a, c.r, c.g, c.b); }

    XAML_API void drawing_context::draw_ellipse(drawing_pen const& pen, rectangle const& region)
    {
        m_handle.DrawEllipse((float)(region.x + region.width / 2), (float)(region.y + region.height / 2), (float)region.width / 2.0f, (float)region.height / 2.0f, get_Color(pen.get_color()), (float)pen.get_width());
    }

    XAML_API canvas::canvas() : common_control() {}
    XAML_API canvas::~canvas() {}

    XAML_API void canvas::__draw(rectangle const& region)
    {
        if (!get_handle())
        {
            CanvasControl cv{};
            cv.Draw([this](CanvasControl const&, CanvasDrawEventArgs const& args) -> void {
                drawing_context dc{ args.DrawingSession() };
                m_redraw(*this, dc);
            });
            set_handle(cv);
        }
        CanvasControl cv = get_handle().as<CanvasControl>();
        cv.Margin(get_Thickness(get_margin()));
    }
} // namespace xaml
