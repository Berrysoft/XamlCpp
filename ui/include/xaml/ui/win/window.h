#ifndef XAML_UI_WINDOWS_WINDOW_H
#define XAML_UI_WINDOWS_WINDOW_H

#include <Windows.h>
#include <xaml/ui/win/control.h>
#include <xaml/ui/window.h>

XAML_CLASS(xaml_win32_window, { 0xa4ab9b2f, 0xad85, 0x498d, { 0xb8, 0x5c, 0xad, 0xb9, 0x68, 0xfb, 0x02, 0xcd } })

#define XAML_WIN32_WINDOW_VTBL(type)                                        \
    XAML_VTBL_INHERIT(XAML_WIN32_CONTROL_VTBL(type));                       \
    XAML_PROP(real_location, type, xaml_point*, xaml_point XAML_CONST_REF); \
    XAML_METHOD(get_real_client_region, type, xaml_rectangle*)

XAML_DECL_INTERFACE_(xaml_win32_window, xaml_win32_control)
{
    XAML_DECL_VTBL(xaml_win32_window, XAML_WIN32_WINDOW_VTBL);
};

EXTERN_C XAML_UI_API LRESULT CALLBACK xaml_window_callback(HWND, UINT, WPARAM, LPARAM) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result XAML_CALL xaml_window_from_native(HWND, xaml_window**) XAML_NOEXCEPT;

#endif // !XAML_UI_WINDOWS_WINDOW_H
