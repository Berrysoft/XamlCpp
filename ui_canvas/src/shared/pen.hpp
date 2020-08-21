#ifndef XAML_UI_CANVAS_SHARED_PEN_HPP
#define XAML_UI_CANVAS_SHARED_PEN_HPP

#include <xaml/ui/controls/pen.h>

#ifdef XAML_UI_WINDOWS
    #include <xaml/ui/win/controls/pen.h>
#elif defined(XAML_UI_COCOA)
    #include <xaml/ui/cocoa/controls/pen.h>
#elif defined(XAML_UI_GTK3)
    #include <xaml/ui/gtk3/controls/pen.h>
#elif defined(XAML_UI_QT5)
    #include <xaml/ui/qt5/controls/pen.hpp>
#endif // XAML_UI_WINDOWS

template <typename T, typename... Base>
struct xaml_pen_implement : xaml_implement<T, Base..., xaml_pen, xaml_object>
{
    XAML_PROP_IMPL(width, double, double*, double)

#ifdef XAML_UI_WINDOWS
    virtual xaml_result XAML_CALL create(ID2D1RenderTarget*, xaml_rectangle const&, ID2D1Brush**, FLOAT*) noexcept = 0;

    struct xaml_win32_pen_impl : xaml_inner_implement<xaml_win32_pen_impl, T, xaml_win32_pen>
    {
        xaml_result XAML_CALL create(ID2D1RenderTarget* target, xaml_rectangle const& region, ID2D1Brush** ptr, FLOAT* pwidth) noexcept override { return this->m_outer->create(target, region, ptr, pwidth); }
    } m_native_pen;

    using native_pen_type = xaml_win32_pen;
#elif defined(XAML_UI_COCOA)
    virtual xaml_result XAML_CALL draw(OBJC_OBJECT(NSBezierPath), xaml_size const&, xaml_rectangle const&) noexcept = 0;

    struct xaml_cocoa_pen_impl : xaml_inner_implement<xaml_cocoa_pen_impl, T, xaml_cocoa_pen>
    {
        xaml_result XAML_CALL draw(OBJC_OBJECT(NSBezierPath) path, xaml_size const& size, xaml_rectangle const& region) noexcept override { return this->m_outer->draw(path, size, region); }
    } m_native_pen;

    using native_pen_type = xaml_cocoa_pen;
#elif defined(XAML_UI_GTK3)
    virtual xaml_result XAML_CALL set(cairo_t*, xaml_rectangle const&) noexcept = 0;

    struct xaml_gtk3_pen_impl : xaml_inner_implement<xaml_gtk3_pen_impl, T, xaml_gtk3_pen>
    {
        xaml_result XAML_CALL set(cairo_t* handle, xaml_rectangle const& region) noexcept override { return this->m_outer->set(handle, region); }
    } m_native_pen;

    using native_pen_type = xaml_gtk3_pen;
#elif defined(XAML_UI_QT5)
    virtual xaml_result XAML_CALL create(xaml_rectangle const&, QPen*) noexcept = 0;

    struct xaml_qt5_pen_impl : xaml_inner_implement<xaml_qt5_pen_impl, T, xaml_qt5_pen>
    {
        xaml_result XAML_CALL create(xaml_rectangle const& region, QPen* ptr) noexcept override { return this->m_outer->set(region, ptr); }
    } m_native_pen;

    using native_pen_type = xaml_qt5_pen;
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

struct xaml_brush_pen_impl : xaml_pen_implement<xaml_brush_pen_impl, xaml_brush_pen>
{
    XAML_PROP_PTR_IMPL(brush, xaml_brush)

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL create(ID2D1RenderTarget*, xaml_rectangle const&, ID2D1Brush**, FLOAT*) noexcept override;
#elif defined(XAML_UI_COCOA)
    xaml_result XAML_CALL draw(OBJC_OBJECT(NSBezierPath), xaml_size const&, xaml_rectangle const&) noexcept override;
#elif defined(XAML_UI_GTK3)
    xaml_result XAML_CALL set(cairo_t*, xaml_rectangle const&) noexcept override;
#elif defined(XAML_UI_QT5)
    xaml_result XAML_CALL create(xaml_rectangle const&, QPen*) noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_brush_pen_impl(xaml_ptr<xaml_brush> const& brush, double width) noexcept : xaml_pen_implement(width), m_brush(brush)
    {
    }
};

#endif // !XAML_UI_CANVAS_SHARED_PEN_HPP
