#ifndef XAML_UI_CANVAS_SHARED_BRUSH_HPP
#define XAML_UI_CANVAS_SHARED_BRUSH_HPP

#include <xaml/ui/controls/brush.h>

#ifdef XAML_UI_WINDOWS
#include <xaml/ui/win/controls/brush.h>
#elif defined(XAML_UI_COCOA)
#include <xaml/ui/cocoa/controls/brush.h>
#elif defined(XAML_UI_GTK3)
#include <xaml/ui/gtk3/controls/brush.h>
#endif // XAML_UI_WINDOWS

template <typename T, typename... Base>
struct xaml_brush_implement : xaml_implement<T, Base..., xaml_brush, xaml_object>
{
#ifdef XAML_UI_WINDOWS
    virtual xaml_result XAML_CALL create(ID2D1RenderTarget*, ID2D1Brush**) noexcept = 0;

    struct xaml_win32_brush_impl : xaml_inner_implement<xaml_win32_brush_impl, T, xaml_win32_brush>
    {
        xaml_result XAML_CALL create(ID2D1RenderTarget* target, ID2D1Brush** ptr) noexcept override { return this->m_outer->create(target, ptr); }
    } m_native_brush;

    using native_brush_type = xaml_win32_brush;
#elif defined(XAML_UI_COCOA)
    virtual xaml_result XAML_CALL set(OBJC_OBJECT(NSBezierPath)) noexcept = 0;

    struct xaml_cocoa_brush_impl : xaml_inner_implement<xaml_cocoa_brush_impl, T, xaml_cocoa_brush>
    {
        xaml_result XAML_CALL set(OBJC_OBJECT(NSBezierPath) path) noexcept override { return this->m_outer->set(path); }
    } m_native_brush;

    using native_brush_type = xaml_cocoa_brush;
#elif defined(XAML_UI_GTK3)
    virtual xaml_result XAML_CALL set(cairo_t*) noexcept = 0;

    struct xaml_gtk3_brush_impl : xaml_inner_implement<xaml_gtk3_brush_impl, T, xaml_gtk3_brush>
    {
        xaml_result XAML_CALL set(cairo_t* handle) noexcept override { return this->m_outer->set(handle); }
    } m_native_brush;

    using native_brush_type = xaml_gtk3_brush;
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
            return xaml_implement<T, Base..., xaml_brush, xaml_object>::query(type, ptr);
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
    xaml_result XAML_CALL create(ID2D1RenderTarget*, ID2D1Brush**) noexcept override;
#elif defined(XAML_UI_COCOA)
    xaml_result XAML_CALL set(OBJC_OBJECT(NSBezierPath)) noexcept override;
#elif defined(XAML_UI_GTK3)
    xaml_result XAML_CALL set(cairo_t*) noexcept override;
#endif // XAML_UI_WINDOWS

    xaml_solid_brush_impl(xaml_color color) noexcept : m_color(color)
    {
    }
};

#endif // !XAML_UI_CANVAS_SHARED_BRUSH_HPP
