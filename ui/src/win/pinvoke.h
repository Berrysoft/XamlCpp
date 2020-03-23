#ifndef XAML_UI_NATIVE_PINVOKE_HPP
#define XAML_UI_NATIVE_PINVOKE_HPP

#include <Windows.h>
#include <xaml/utility.hpp>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    XAML_UI_API BOOL WINAPI XamlSetProcessBestDpiAwareness();

    XAML_UI_API BOOL WINAPI XamlEnableNonClientDpiScaling(HWND hwnd);

    XAML_UI_API BOOL WINAPI XamlSystemDefaultFontForDpi(LPLOGFONT lfFont, UINT dpi);

    XAML_UI_API int WINAPI XamlGetSystemMetricsForDpi(int nIndex, UINT dpi);

    XAML_UI_API UINT WINAPI XamlGetDpiForWindow(HWND hWnd);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !XAML_UI_NATIVE_PINVOKE_HPP
