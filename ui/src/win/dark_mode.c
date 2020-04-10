#include <xaml/ui/win/dark_mode.h>

#include <Uxtheme.h>
#include <dwmapi.h>

NTSYSAPI void NTAPI RtlGetNtVersionNumbers(LPDWORD, LPDWORD, LPDWORD);

typedef BOOLEAN(WINAPI* pfShouldAppsUseDarkMode)(void);
static pfShouldAppsUseDarkMode pShouldAppsUseDarkMode;

typedef BOOLEAN(WINAPI* pfAllowDarkModeForApp)(BOOLEAN);
static pfAllowDarkModeForApp pAllowDarkModeForApp;

typedef XAML_PREFERRED_APP_MODE(WINAPI* pfSetPreferredAppMode)(XAML_PREFERRED_APP_MODE);
static pfSetPreferredAppMode pSetPreferredAppMode;

typedef BOOLEAN(WINAPI* pfAllowDarkModeForWindow)(HWND, BOOLEAN);
static pfAllowDarkModeForWindow pAllowDarkModeForWindow;

typedef void(WINAPI* pfFlushMenuThemes)(void);
static pfFlushMenuThemes pFlushMenuThemes;

static HMODULE uxtheme = NULL;

void WINAPI XamlInitializeDarkModeFunc(void)
{
    if (!uxtheme)
    {
        uxtheme = LoadLibraryEx(L"Uxtheme.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (uxtheme)
        {
            DWORD build;
            RtlGetNtVersionNumbers(NULL, NULL, &build);
            build &= ~0xF0000000;
            pShouldAppsUseDarkMode = (pfShouldAppsUseDarkMode)GetProcAddress(uxtheme, MAKEINTRESOURCEA(132));
            pAllowDarkModeForWindow = (pfAllowDarkModeForWindow)GetProcAddress(uxtheme, MAKEINTRESOURCEA(133));
            if (build < 18362)
                pAllowDarkModeForApp = (pfAllowDarkModeForApp)GetProcAddress(uxtheme, MAKEINTRESOURCEA(135));
            else
                pSetPreferredAppMode = (pfSetPreferredAppMode)GetProcAddress(uxtheme, MAKEINTRESOURCEA(135));
            pFlushMenuThemes = (pfFlushMenuThemes)GetProcAddress(uxtheme, MAKEINTRESOURCEA(136));
        }
    }
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

BOOL WINAPI XamlIsDarkModeAllowedForApp(void)
{
    if (pShouldAppsUseDarkMode)
    {
        HIGHCONTRAST hc = { 0 };
        hc.cbSize = sizeof(hc);
        if (!SystemParametersInfo(SPI_GETHIGHCONTRAST, sizeof(hc), &hc, 0)) return FALSE;
        return (!(hc.dwFlags & HCF_HIGHCONTRASTON)) && pShouldAppsUseDarkMode();
    }
    return FALSE;
}

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE
#define DWMWA_USE_IMMERSIVE_DARK_MODE 0x13
#endif // !DWMWA_USE_IMMERSIVE_DARK_MODE

#ifndef DWMWA_USE_IMMERSIVE_DARK_MODE_V2
#define DWMWA_USE_IMMERSIVE_DARK_MODE_V2 0x14
#endif // !DWMWA_USE_IMMERSIVE_DARK_MODE_V2

HRESULT WINAPI XamlWindowUseDarkMode(HWND hWnd)
{
    if (pAllowDarkModeForWindow) pAllowDarkModeForWindow(hWnd, TRUE);
    BOOL set_dark_mode = TRUE;
    if (FAILED(DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE_V2, &set_dark_mode, sizeof(BOOL))))
    {
        HRESULT hr = DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &set_dark_mode, sizeof(BOOL));
        if (FAILED(hr)) return hr;
    }
    if (pFlushMenuThemes) pFlushMenuThemes();
    return S_OK;
}

HRESULT WINAPI XamlControlUseDarkMode(HWND hWnd)
{
    return SetWindowTheme(hWnd, L"DarkMode_Explorer", NULL);
}
