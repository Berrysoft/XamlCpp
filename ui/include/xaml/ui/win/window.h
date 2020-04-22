#ifndef XAML_UI_WINDOWS_WINDOW_H
#define XAML_UI_WINDOWS_WINDOW_H

#include <Windows.h>
#include <xaml/ui/window.h>

EXTERN_C XAML_UI_API LRESULT CALLBACK xaml_window_callback(HWND, UINT, WPARAM, LPARAM) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result xaml_window_from_native(HWND, xaml_window**) XAML_NOEXCEPT;

#endif // !XAML_UI_WINDOWS_WINDOW_H
