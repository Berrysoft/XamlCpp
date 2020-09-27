#ifndef XAML_UI_CANVAS_SHARED_BRUSH_HPP
#define XAML_UI_CANVAS_SHARED_BRUSH_HPP

#include <xaml/ui/controls/brush.h>

#ifdef XAML_UI_WINDOWS
    #include <xaml/ui/win/controls/brush.h>
#elif defined(XAML_UI_COCOA)
    #include <xaml/ui/cocoa/controls/brush.h>
#elif defined(XAML_UI_GTK3)
    #include <xaml/ui/gtk3/controls/brush.h>
#elif defined(XAML_UI_QT5)
    #include <xaml/ui/qt5/controls/brush.hpp>
#endif // XAML_UI_WINDOWS

template <typename T, typename Base>
struct xaml_brush_implement : xaml_implement<T, Base>
{
#ifdef XAML_UI_WINDOWS
    virtual xaml_result XAML_CALL create(ID2D1RenderTarget*, xaml_rectangle const&, ID2D1Brush**) noexcept = 0;

    struct xaml_win32_brush_impl : xaml_inner_implement<xaml_win32_brush_impl, T, xaml_win32_brush>
    {
        xaml_result XAML_CALL create(ID2D1RenderTarget* target, xaml_rectangle const& region, ID2D1Brush** ptr) noexcept override { return this->m_outer->create(target, region, ptr); }
    } m_native_brush;

    using native_brush_type = xaml_win32_brush;
#elif defined(XAML_UI_COCOA)
    virtual xaml_result XAML_CALL draw(OBJC_OBJECT(NSBezierPath), xaml_size const&, xaml_rectangle const&) noexcept = 0;

    struct xaml_cocoa_brush_impl : xaml_inner_implement<xaml_cocoa_brush_impl, T, xaml_cocoa_brush>
    {
        xaml_result XAML_CALL draw(OBJC_OBJECT(NSBezierPath) path, xaml_size const& size, xaml_rectangle const& region) noexcept override { return this->m_outer->draw(path, size, region); }
    } m_native_brush;

    using native_brush_type = xaml_cocoa_brush;
#elif defined(XAML_UI_GTK3)
    virtual xaml_result XAML_CALL set(cairo_t*, xaml_rectangle const&) noexcept = 0;

    struct xaml_gtk3_brush_impl : xaml_inner_implement<xaml_gtk3_brush_impl, T, xaml_gtk3_brush>
    {
        xaml_result XAML_CALL set(cairo_t* handle, xaml_rectangle const& region) noexcept override { return this->m_outer->set(handle, region); }
    } m_native_brush;

    using native_brush_type = xaml_gtk3_brush;
#elif defined(XAML_UI_QT5)
    virtual xaml_result XAML_CALL create(xaml_rectangle const&, QBrush*) noexcept = 0;

    struct xaml_qt5_brush_impl : xaml_inner_implement<xaml_qt5_brush_impl, T, xaml_qt5_brush>
    {
        xaml_result XAML_CALL create(xaml_rectangle const& region, QBrush* ptr) noexcept override { return this->m_outer->create(region, ptr); }
    } m_native_brush;

    using native_brush_type = xaml_qt5_brush;
#endif // XAML_UI_WINDOWS

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override
    {
        if (type == xaml_type_guid_v<native_brush_type>)
        {
            this->add_ref();
            *ptr = static_cast<native_brush_type*>(&m_native_brush);
            return XAML_S_OK;
        }
        else
        {
            return xaml_implement<T, Base>::query(type, ptr);
        }
    }

    xaml_brush_implement() noexcept
    {
        m_native_brush.m_outer = static_cast<T*>(this);
    }
};

struct xaml_solid_brush_impl : xaml_brush_implement<xaml_solid_brush_impl, xaml_solid_brush>
{
    XAML_PROP_IMPL(color, xaml_color, xaml_color*, xaml_color)

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL create(ID2D1RenderTarget*, xaml_rectangle const&, ID2D1Brush**) noexcept override;
#elif defined(XAML_UI_COCOA)
    xaml_result XAML_CALL draw(OBJC_OBJECT(NSBezierPath), xaml_size const&, xaml_rectangle const&) noexcept override;
#elif defined(XAML_UI_GTK3)
    xaml_result XAML_CALL set(cairo_t*, xaml_rectangle const&) noexcept override;
#elif defined(XAML_UI_QT5)
    xaml_result XAML_CALL create(xaml_rectangle const&, QBrush*) noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_solid_brush_impl(xaml_color color) noexcept : m_color(color)
    {
    }
};

template <typename T, typename Base>
struct xaml_gradient_brush_implement : xaml_brush_implement<T, Base>
{
    xaml_ptr<xaml_vector<xaml_gradient_stop>> m_gradient_stops;

    xaml_result XAML_CALL get_stops(xaml_vector_view<xaml_gradient_stop>** ptr) noexcept override
    {
        return m_gradient_stops.query(ptr);
    }

    xaml_result XAML_CALL add_stop(xaml_gradient_stop const& stop) noexcept override
    {
        return m_gradient_stops->append(stop);
    }

    xaml_result XAML_CALL remove_stop(xaml_gradient_stop const& stop) noexcept override
    {
        std::int32_t index;
        XAML_RETURN_IF_FAILED(m_gradient_stops->index_of(stop, &index));
        if (index != -1)
        {
            return m_gradient_stops->remove_at(index);
        }
        return XAML_S_OK;
    }

    virtual xaml_result XAML_CALL init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&m_gradient_stops));

        return XAML_S_OK;
    }
};

struct xaml_linear_gradient_brush_impl : xaml_gradient_brush_implement<xaml_linear_gradient_brush_impl, xaml_linear_gradient_brush>
{
    XAML_PROP_IMPL(start_point, xaml_point, xaml_point*, xaml_point const&)
    XAML_PROP_IMPL(end_point, xaml_point, xaml_point*, xaml_point const&)

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL create(ID2D1RenderTarget*, xaml_rectangle const&, ID2D1Brush**) noexcept override;
#elif defined(XAML_UI_COCOA)
    xaml_result XAML_CALL draw(OBJC_OBJECT(NSBezierPath), xaml_size const&, xaml_rectangle const&) noexcept override;
#elif defined(XAML_UI_GTK3)
    xaml_result XAML_CALL set(cairo_t*, xaml_rectangle const&) noexcept override;
#elif defined(XAML_UI_QT5)
    xaml_result XAML_CALL create(xaml_rectangle const&, QBrush*) noexcept override;
#endif // XAML_UI_WINDOWS
};

struct xaml_radial_gradient_brush_impl : xaml_gradient_brush_implement<xaml_radial_gradient_brush_impl, xaml_radial_gradient_brush>
{
    XAML_PROP_IMPL(origin, xaml_point, xaml_point*, xaml_point const&)
    XAML_PROP_IMPL(center, xaml_point, xaml_point*, xaml_point const&)
    XAML_PROP_IMPL(radius, xaml_size, xaml_size*, xaml_size const&)

#ifdef XAML_UI_WINDOWS
    xaml_result XAML_CALL create(ID2D1RenderTarget*, xaml_rectangle const&, ID2D1Brush**) noexcept override;
#elif defined(XAML_UI_COCOA)
    xaml_result XAML_CALL draw(OBJC_OBJECT(NSBezierPath), xaml_size const&, xaml_rectangle const&) noexcept override;
#elif defined(XAML_UI_GTK3)
    xaml_result XAML_CALL set(cairo_t*, xaml_rectangle const&) noexcept override;
#elif defined(XAML_UI_QT5)
    xaml_result XAML_CALL create(xaml_rectangle const&, QBrush*) noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_radial_gradient_brush_impl() noexcept : m_origin({ 0.5, 0.5 }), m_center({ 0.5, 0.5 }), m_radius({ 0.5, 0.5 })
    {
    }
};

#endif // !XAML_UI_CANVAS_SHARED_BRUSH_HPP
