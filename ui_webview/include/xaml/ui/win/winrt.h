#ifndef XAML_INTERNAL_WINRT_H
#define XAML_INTERNAL_WINRT_H

#include <Windows.h>
#include <xaml/utility.h>

#ifndef __cplusplus
    #include <stdbool.h>
#endif // !__cplusplus

#ifndef XAML_WINRT_API
    #define XAML_WINRT_API __XAML_IMPORT
#endif // !XAML_WINRT_API

EXTERN_C XAML_UI_WEBVIEW_API bool WINAPI XamlInitializeWinRTFunc(void) XAML_NOEXCEPT;

#endif // !XAML_INTERNAL_WINRT_H
