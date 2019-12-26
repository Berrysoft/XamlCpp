#include <xaml/ui/canvas.hpp>

using namespace std;

#if defined(XAML_UI_GTK3) || defined(XAML_UI_COCOA)
namespace xaml
{
    drawing_brush::drawing_brush(color c) : m_object({ c }) {}

    color drawing_brush::get_color() const { return m_object.fill; }

    void drawing_brush::set_color(color value) { m_object.fill = value; }

    drawing_pen::drawing_pen(color c, double width) : m_object({ c, width }) {}

    color drawing_pen::get_color() const { return m_object.stroke; }

    void drawing_pen::set_color(color value) { m_object.stroke = value; }

    double drawing_pen::get_width() const { return m_object.width; }

    void drawing_pen::set_width(double value) { m_object.width = value; }

    drawing_font::drawing_font(string_view_t family, double size, bool italic, bool bold) : m_object({ (string_t)family, size, italic, bold }) {}

    string_view_t drawing_font::get_font_family() const { return m_object.font_family; }

    void drawing_font::set_font_family(string_view_t value) { m_object.font_family = (string_t)value; }

    double drawing_font::get_size() const { return m_object.size; }

    bool drawing_font::get_italic() const { return m_object.italic; }

    bool drawing_font::get_bold() const { return m_object.bold; }
} // namespace xaml
#endif
