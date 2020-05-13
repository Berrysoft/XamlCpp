#ifndef XAML_UI_WEBVIEW_TEST_WINDOW_HPP
#define XAML_UI_WEBVIEW_TEST_WINDOW_HPP

#include <xaml/object.h>

XAML_CLASS(xaml_test_window, { 0xc609d8d6, 0x466c, 0x4920, { 0x8f, 0x88, 0x55, 0x99, 0x5d, 0xa5, 0xe6, 0x35 } })

#define XAML_TEST_WINDOW_VTBL(type)            \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(show, type)

XAML_DECL_INTERFACE_(xaml_test_window, xaml_object)
{
    XAML_DECL_VTBL(xaml_test_window, XAML_TEST_WINDOW_VTBL);
};

EXTERN_C xaml_result XAML_CALL xaml_test_window_new(xaml_test_window**) XAML_NOEXCEPT;

#endif // !XAML_UI_WEBVIEW_TEST_WINDOW_HPP
