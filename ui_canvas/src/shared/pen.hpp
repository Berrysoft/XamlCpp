#ifndef XAML_UI_CANVAS_SHARED_PEN_HPP
#define XAML_UI_CANVAS_SHARED_PEN_HPP

#include <xaml/ui/controls/pen.h>

#ifdef XAML_UI_WINDOWS
#include <xaml/ui/win/controls/pen.h>
#endif // XAML_UI_WINDOWS

template <typename T, typename... Base>
struct xaml_pen_implement : xaml_implement<T, Base..., xaml_pen, xaml_object>
{
    XAML_PROP_IMPL(width, double, double*, double)

#ifdef XAML_UI_WINDOWS
    virtual xaml_result XAML_CALL create(ID2D1RenderTarget*, ID2D1Brush**, FLOAT*) noexcept = 0;

    struct xaml_win32_pen_impl : xaml_inner_implement<xaml_win32_pen_impl, T, xaml_win32_pen>
    {
        xaml_result XAML_CALL create(ID2D1RenderTarget* target, ID2D1Brush** ptr, FLOAT* pwidth) noexcept override { return this->m_outer->create(target, ptr, pwidth); }
    } m_native_pen;

    using native_pen_type = xaml_win32_pen;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<native_pen_type>)
        {
            this->add_ref();
            *ptr = static_cast<native_pen_type*>(&m_native_pen);
            return XAML_S_OK;
        }
        else
        {
            return xaml_implement<T, Base..., xaml_pen, xaml_object>::query(type, ptr);
        }
    }

    xaml_pen_implement(double width) noexcept : m_width(width)
    {
        m_native_pen.m_outer = static_cast<T*>(this);
    }
};

struct xaml_solid_pen_impl : xaml_pen_implement<xaml_solid_pen_impl, xaml_solid_pen>
{
    XAML_PROP_IMPL(color, xaml_color, xaml_color*, xaml_color)

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL create(ID2D1RenderTarget*, ID2D1Brush**, FLOAT*) noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_solid_pen_impl(xaml_color color, double width) noexcept : xaml_pen_implement(width), m_color(color)
    {
    }
};

#endif // !XAML_UI_CANVAS_SHARED_PEN_HPP
