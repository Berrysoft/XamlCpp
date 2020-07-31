#ifndef XAML_UI_NATIVE_PINVOKE_HPP
#define XAML_UI_NATIVE_PINVOKE_HPP

#include <Windows.h>
#include <xaml/utility.h>

EXTERN_C_START

XAML_UI_API void WINAPI XamlInitializeDpiFunc(void) XAML_NOEXCEPT;

XAML_UI_API BOOL WINAPI XamlSetProcessBestDpiAwareness(void) XAML_NOEXCEPT;

XAML_UI_API BOOL WINAPI XamlEnableNonClientDpiScaling(HWND hwnd) XAML_NOEXCEPT;

XAML_UI_API BOOL WINAPI XamlSystemDefaultFontForDpi(LPLOGFONT lfFont, UINT dpi) XAML_NOEXCEPT;

XAML_UI_API int WINAPI XamlGetSystemMetricsForDpi(int nIndex, UINT dpi) XAML_NOEXCEPT;

XAML_UI_API UINT WINAPI XamlGetDpiForWindow(HWND hWnd) XAML_NOEXCEPT;

EXTERN_C_END

#endif // !XAML_UI_NATIVE_PINVOKE_HPP
