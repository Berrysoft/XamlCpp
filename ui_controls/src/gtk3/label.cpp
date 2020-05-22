#include <shared/label.hpp>
#include <xaml/ui/controls/label.h>

using namespace std;

xaml_result xaml_label_internal::draw(xaml_rectangle const& region) noexcept
{
    if (!m_handle)
    {
        char const* text = nullptr;
        if (m_text)
        {
            XAML_RETURN_IF_FAILED(m_text->get_data(&text));
        }
        m_handle = gtk_label_new(text);
        XAML_RETURN_IF_FAILED(draw_visible());
        XAML_RETURN_IF_FAILED(draw_text());
        XAML_RETURN_IF_FAILED(draw_alignment());
    }
    return set_rect(region);
}

xaml_result xaml_label_internal::draw_text() noexcept
{
    char const* data = nullptr;
    if (m_text)
    {
        XAML_RETURN_IF_FAILED(m_text->get_data(&data));
    }
    gtk_label_set_label(GTK_LABEL(m_handle), data);
    return XAML_S_OK;
}

xaml_result xaml_label_internal::draw_alignment() noexcept
{
    gfloat align;
    switch (m_text_halignment)
    {
    case xaml_halignment_center:
        align = 0.5;
        break;
    case xaml_halignment_right:
        align = 1.0;
        break;
    default:
        align = 0;
        break;
    }
    gtk_label_set_xalign(GTK_LABEL(m_handle), align);
    return XAML_S_OK;
}
