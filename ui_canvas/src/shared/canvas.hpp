#ifndef XAML_UI_CANVAS_SHARED_CANVAS_HPP
#define XAML_UI_CANVAS_SHARED_CANVAS_HPP

#include <shared/control.hpp>
#include <xaml/ui/controls/canvas.h>

#ifdef XAML_UI_WINDOWS
    #include <Windows.h>
    #include <d2d1.h>
    #include <dwrite.h>
    #include <wil/com.h>

template <typename Factory>
HRESULT DWriteCreateFactory(DWRITE_FACTORY_TYPE factoryType, Factory** factory) noexcept
{
    return DWriteCreateFactory(factoryType, __uuidof(Factory), reinterpret_cast<IUnknown**>(factory));
}

#elif defined(XAML_UI_COCOA)
    #include <xaml/ui/cocoa/objc.h>
#elif defined(XAML_UI_GTK3)
    #include <cairo.h>
#elif defined(XAML_UI_QT5)
    #include <QPaintEvent>
    #include <QPainter>
#endif // XAML_UI_GTK3

struct xaml_drawing_context_impl : xaml_implement<xaml_drawing_context_impl, xaml_drawing_context>
{
#ifdef XAML_UI_WINDOWS
    wil::com_ptr_nothrow<ID2D1RenderTarget> target{ nullptr };
    wil::com_ptr_nothrow<ID2D1Factory> d2d{ nullptr };
    wil::com_ptr_nothrow<IDWriteFactory> dwrite{ nullptr };

    xaml_drawing_context_impl(wil::com_ptr_nothrow<ID2D1RenderTarget> const& target, wil::com_ptr_nothrow<ID2D1Factory> const& d2d, wil::com_ptr_nothrow<IDWriteFactory> const& dwrite) noexcept
        : target(target), d2d(d2d), dwrite(dwrite) {}
#elif defined(XAML_UI_COCOA)
    xaml_size m_size;

    xaml_drawing_context_impl(xaml_size const& size) noexcept : m_size(size) {}
#elif defined(XAML_UI_GTK3)
    cairo_t* m_handle;

    xaml_drawing_context_impl(cairo_t* handle) noexcept : m_handle(handle) {}
#elif defined(XAML_UI_QT5)
    QPainter* m_handle;

    xaml_drawing_context_impl(QPainter* handle) noexcept : m_handle(handle) {}
#endif // XAML_UI_GTK3

    xaml_result XAML_CALL draw_arc(xaml_pen* pen, xaml_rectangle const& region, double start_angle, double end_angle) noexcept override;
    xaml_result XAML_CALL fill_pie(xaml_brush* brush, xaml_rectangle const& region, double start_angle, double end_angle) noexcept override;
    xaml_result XAML_CALL draw_ellipse(xaml_pen* pen, xaml_rectangle const& region) noexcept override;
    xaml_result XAML_CALL fill_ellipse(xaml_brush* brush, xaml_rectangle const& region) noexcept override;
    xaml_result XAML_CALL draw_line(xaml_pen* pen, xaml_point const& startp, xaml_point const& endp) noexcept override;
    xaml_result XAML_CALL draw_rect(xaml_pen* pen, xaml_rectangle const& rect) noexcept override;
    xaml_result XAML_CALL fill_rect(xaml_brush* brush, xaml_rectangle const& rect) noexcept override;
    xaml_result XAML_CALL draw_round_rect(xaml_pen* pen, xaml_rectangle const& rect, xaml_size const& round) noexcept override;
    xaml_result XAML_CALL fill_round_rect(xaml_brush* brush, xaml_rectangle const& rect, xaml_size const& round) noexcept override;
    xaml_result XAML_CALL draw_string(xaml_brush* brush, xaml_drawing_font const& font, xaml_point const& p, xaml_string* str) noexcept override;
};

struct xaml_canvas_internal : xaml_control_internal
{
    XAML_EVENT_IMPL(redraw)

    xaml_result XAML_CALL draw(xaml_rectangle const&) noexcept override;

    xaml_result XAML_CALL invalidate(xaml_rectangle const*) noexcept;

#ifdef XAML_UI_WINDOWS
    wil::com_ptr_nothrow<ID2D1HwndRenderTarget> target{ nullptr };
    wil::com_ptr_nothrow<ID2D1Factory> d2d{ nullptr };
    wil::com_ptr_nothrow<IDWriteFactory> dwrite{ nullptr };

    xaml_result XAML_CALL wnd_proc(xaml_win32_window_message const&, LRESULT*) noexcept override;
#elif defined(XAML_UI_COCOA)
    void on_draw_rect() noexcept;
#elif defined(XAML_UI_GTK3)
    static gboolean on_draw(GtkWidget*, cairo_t*, xaml_canvas_internal*) noexcept;
#elif defined(XAML_UI_QT5)
    void on_paint_event(QPaintEvent*) noexcept;
    void on_mouse_move_event(QMouseEvent*) noexcept;
    void on_mouse_press_event(QMouseEvent*) noexcept;
    void on_mouse_release_event(QMouseEvent*) noexcept;
#endif // XAML_UI_GTK3

    xaml_result XAML_CALL init() noexcept override;
};

struct xaml_canvas_impl : xaml_control_implement<xaml_canvas_impl, xaml_canvas_internal, xaml_canvas>
{
    XAML_EVENT_INTERNAL_IMPL(redraw)

    xaml_result XAML_CALL invalidate() noexcept override { return m_internal.invalidate(nullptr); }
    xaml_result XAML_CALL invalidate_rect(xaml_rectangle const& rect) noexcept override { return m_internal.invalidate(&rect); }
};

#endif // !XAML_UI_CANVAS_SHARED_CANVAS_HPP
