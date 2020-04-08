#include <win/result_macros.h>
#include <xaml/ui/win/dark_mode.h>

#include <Uxtheme.h>
#include <dwmapi.h>

void WINAPI RtlGetNtVersionNumbers(LPDWORD, LPDWORD, LPDWORD);

typedef BOOL(WINAPI* pfShouldUseDarkMode)();
static pfShouldUseDarkMode pShouldSystemUseDarkMode;
static pfShouldUseDarkMode pShouldAppUseDarkMode;

typedef BOOL(WINAPI* pfAllowDarkModeForApp)(BOOL);
static pfAllowDarkModeForApp pAllowDarkModeForApp;

typedef XAML_PREFERRED_APP_MODE(WINAPI* pfSetPreferredAppMode)(XAML_PREFERRED_APP_MODE);
static pfSetPreferredAppMode pSetPreferredAppMode;

typedef BOOL(WINAPI* pfIsDarkModeAllowedForApp)();
static pfIsDarkModeAllowedForApp pIsDarkModeAllowedForApp;

typedef void(WINAPI* pfFlushMenuThemes)();
static pfFlushMenuThemes pFlushMenuThemes;

static HMODULE uxtheme = NULL;

static DWORD get_build_version()
{
    DWORD build;
    RtlGetNtVersionNumbers(NULL, NULL, &build);
    return build & ~0xF0000000;
}

void WINAPI XamlInitializeDarkModeFunc()
{
    if (!uxtheme)
    {
        uxtheme = LoadLibraryEx(L"Uxtheme.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (uxtheme)
        {
            DWORD build = get_build_version();
            pShouldSystemUseDarkMode = (pfShouldUseDarkMode)GetProcAddress(uxtheme, MAKEINTRESOURCEA(138));
            pShouldAppUseDarkMode = (pfShouldUseDarkMode)GetProcAddress(uxtheme, MAKEINTRESOURCEA(132));
            if (build < 18362)
                pAllowDarkModeForApp = (pfAllowDarkModeForApp)GetProcAddress(uxtheme, MAKEINTRESOURCEA(135));
            else
                pSetPreferredAppMode = (pfSetPreferredAppMode)GetProcAddress(uxtheme, MAKEINTRESOURCEA(135));
            pIsDarkModeAllowedForApp = (pfIsDarkModeAllowedForApp)GetProcAddress(uxtheme, MAKEINTRESOURCEA(139));
            pFlushMenuThemes = (pfFlushMenuThemes)GetProcAddress(uxtheme, MAKEINTRESOURCEA(136));
        }
    }
}

BOOL WINAPI XamlShouldSystemUseDarkMode()
{
    if (pShouldSystemUseDarkMode)
    {
        return pShouldSystemUseDarkMode();
    }
    else
    {
        return XamlShouldAppUseDarkMode();
    }
}

BOOL WINAPI XamlShouldAppUseDarkMode()
{
    return pShouldAppUseDarkMode && pShouldAppUseDarkMode();
}

XAML_PREFERRED_APP_MODE WINAPI XamlSetPreferredAppMode(XAML_PREFERRED_APP_MODE value)
{
    if (pSetPreferredAppMode)
    {
        return pSetPreferredAppMode(value);
    }
    else if (pAllowDarkModeForApp)
    {
        BOOL result = pAllowDarkModeForApp(value == XAML_PREFERRED_APP_MODE_ALLOW_DARK || value == XAML_PREFERRED_APP_MODE_FORCE_DARK);
        return result ? XAML_PREFERRED_APP_MODE_ALLOW_DARK : XAML_PREFERRED_APP_MODE_DEFAULT;
    }
    return XAML_PREFERRED_APP_MODE_DEFAULT;
}

BOOL WINAPI XamlIsDarkModeAllowedForApp()
{
    return pIsDarkModeAllowedForApp && pIsDarkModeAllowedForApp();
}

BOOL WINAPI XamlIsDarkModeEnabledForApp()
{
    HIGHCONTRAST hc = { 0 };
    hc.cbSize = sizeof(hc);
    RETURN_IF_WIN32_BOOL_FALSE(SystemParametersInfo(SPI_GETHIGHCONTRAST, sizeof(hc), &hc, 0));
    if (hc.dwFlags & HCF_HIGHCONTRASTON) return FALSE;
    DWORD build = get_build_version();
    return (build < 18362 ? XamlShouldAppUseDarkMode() : XamlShouldSystemUseDarkMode()) && XamlIsDarkModeAllowedForApp();
}

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 0x13
#endif // !DWMWA_USE_IMMERSIVE_DARK_MODE

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE_V2
#define DWMWA_USE_IMMERSIVE_DARK_MODE_V2 0x14
#endif // !DWMWA_USE_IMMERSIVE_DARK_MODE_V2

HRESULT WINAPI XamlWindowUseDarkMode(HWND hWnd)
{
    BOOL set_dark_mode = TRUE;
    if (FAILED(DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &set_dark_mode, sizeof(BOOL))))
    {
        RETURN_IF_FAILED(DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE_V2, &set_dark_mode, sizeof(BOOL)));
    }
    SendMessage(hWnd, WM_THEMECHANGED, 0, 0);
    if (pFlushMenuThemes) pFlushMenuThemes();
    return S_OK;
}

HRESULT WINAPI XamlControlUseDarkMode(HWND hWnd)
{
    return SetWindowTheme(hWnd, L"DarkMode_Explorer", NULL);
}
