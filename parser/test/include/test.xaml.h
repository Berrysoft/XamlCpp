#ifndef XAML_TEST_TEST_WINDOW_HPP
#define XAML_TEST_TEST_WINDOW_HPP

#include <test_model.h>
#include <xaml/meta/meta_context.h>
#include <xaml/ui/controls/button.h>
#include <xaml/ui/controls/canvas.h>
#include <xaml/ui/window.h>

XAML_CLASS(xaml_test_window, { 0xc609d8d6, 0x466c, 0x4920, { 0x8f, 0x88, 0x55, 0x99, 0x5d, 0xa5, 0xe6, 0x35 } })

#define XAML_TEST_WINDOW_VTBL(type)                              \
    XAML_VTBL_INHERIT(XAML_WINDOW_VTBL(type));                   \
    XAML_PROP(model, type, xaml_test_model**, xaml_test_model*); \
    XAML_METHOD(on_button_click, type, xaml_button*);            \
    XAML_METHOD(on_canvas_redraw, type, xaml_canvas*, xaml_drawing_context*)

XAML_DECL_INTERFACE_(xaml_test_window, xaml_window)
{
    XAML_DECL_VTBL(xaml_test_window, XAML_TEST_WINDOW_VTBL);
};

EXTERN_C xaml_result XAML_CALL xaml_test_window_new(xaml_meta_context*, xaml_test_window**) XAML_NOEXCEPT;
EXTERN_C xaml_result XAML_CALL xaml_test_window_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_TEST_TEST_WINDOW_HPP
