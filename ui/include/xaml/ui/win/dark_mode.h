#ifndef XAML_UI_NATIVE_WINDOWS_DARK_MODE_H
#define XAML_UI_NATIVE_WINDOWS_DARK_MODE_H

#include <Windows.h>
#include <xaml/utility.hpp>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

    XAML_UI_API void WINAPI XamlInitializeDarkModeFunc();

    XAML_UI_API BOOL WINAPI XamlShouldSystemUseDarkMode();

    XAML_UI_API BOOL WINAPI XamlShouldAppUseDarkMode();

    enum XAML_PREFERRED_APP_MODE
    {
        XAML_PREFERRED_APP_MODE_DEFAULT,
        XAML_PREFERRED_APP_MODE_ALLOW_DARK,
        XAML_PREFERRED_APP_MODE_FORCE_DARK,
        XAML_PREFERRED_APP_MODE_FORCE_LIGHT
    };

    XAML_UI_API XAML_PREFERRED_APP_MODE WINAPI XamlSetPreferredAppMode(XAML_PREFERRED_APP_MODE value);

    XAML_UI_API BOOL WINAPI XamlIsDarkModeAllowedForApp();

    XAML_UI_API HRESULT WINAPI XamlWindowUseDarkMode(HWND hWnd);

    XAML_UI_API HRESULT WINAPI XamlControlUseDarkMode(HWND hWnd);

    inline BOOL XamlIsDarkModeEnabledForApp()
    {
        return XamlShouldAppUseDarkMode() && XamlIsDarkModeAllowedForApp();
    }

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !XAML_UI_NATIVE_WINDOWS_DARK_MODE_H
