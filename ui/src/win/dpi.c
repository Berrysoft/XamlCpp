#include <ShellScalingApi.h>
#include <assert.h>
#include <xaml/ui/win/dpi.h>

typedef BOOL(WINAPI* pfSetProcessDPIAware)();
static pfSetProcessDPIAware pSetProcessDPIAware = NULL;

typedef HRESULT(WINAPI* pfSetProcessDpiAwareness)(PROCESS_DPI_AWARENESS);
static pfSetProcessDpiAwareness pSetProcessDpiAwareness = NULL;

typedef BOOL(WINAPI* pfSetProcessDpiAwarenessContext)(DPI_AWARENESS_CONTEXT);
static pfSetProcessDpiAwarenessContext pSetProcessDpiAwarenessContext = NULL;

typedef BOOL(WINAPI* pfEnableNonClientDpiScaling)(HWND);
static pfEnableNonClientDpiScaling pEnableNonClientDpiScaling = NULL;

typedef BOOL(WINAPI* pfSystemParametersInfoForDpi)(UINT, UINT, PVOID, UINT, UINT);
static pfSystemParametersInfoForDpi pSystemParametersInfoForDpi = NULL;

typedef int(WINAPI* pfGetSystemMetricsForDpi)(int, UINT);
static pfGetSystemMetricsForDpi pGetSystemMetricsForDpi = NULL;

typedef UINT(WINAPI* pfGetDpiForSystem)();
static pfGetDpiForSystem pGetDpiForSystem = NULL;

typedef UINT(WINAPI* pfGetDpiForWindow)(HWND);
static pfGetDpiForWindow pGetDpiForWindow = NULL;

static HMODULE user32 = NULL;
static HMODULE shcore = NULL;

void WINAPI XamlInitializeDpiFunc()
{
    if (!user32)
    {
        user32 = LoadLibraryEx(L"User32.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (user32)
        {
            pSetProcessDPIAware = (pfSetProcessDPIAware)GetProcAddress(user32, "SetProcessDPIAware");
            pSetProcessDpiAwarenessContext = (pfSetProcessDpiAwarenessContext)GetProcAddress(user32, "SetProcessDpiAwarenessContext");
            pEnableNonClientDpiScaling = (pfEnableNonClientDpiScaling)GetProcAddress(user32, "EnableNonClientDpiScaling");
            pSystemParametersInfoForDpi = (pfSystemParametersInfoForDpi)GetProcAddress(user32, "SystemParametersInfoForDpi");
            pGetSystemMetricsForDpi = (pfGetSystemMetricsForDpi)GetProcAddress(user32, "GetSystemMetricsForDpi");
            pGetDpiForSystem = (pfGetDpiForSystem)GetProcAddress(user32, "GetDpiForSystem");
            pGetDpiForWindow = (pfGetDpiForWindow)GetProcAddress(user32, "GetDpiForWindow");
        }
    }
    if (!shcore)
    {
        shcore = LoadLibraryEx(L"Shcore.dll", NULL, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (shcore)
        {
            pSetProcessDpiAwareness = (pfSetProcessDpiAwareness)GetProcAddress(shcore, "SetProcessDpiAwareness");
        }
    }
}

BOOL WINAPI XamlSetProcessBestDpiAwareness()
{
    if (pSetProcessDpiAwarenessContext)
    {
        return pSetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    }
    else if (pSetProcessDpiAwareness)
    {
        HRESULT hr = pSetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
        return SUCCEEDED(hr) || hr == E_ACCESSDENIED;
    }
    else if (pSetProcessDPIAware)
    {
        return pSetProcessDPIAware();
    }
    else
    {
        return TRUE;
    }
}

BOOL WINAPI XamlEnableNonClientDpiScaling(HWND hwnd)
{
    if (pEnableNonClientDpiScaling)
    {
        return pEnableNonClientDpiScaling(hwnd);
    }
    else
    {
        return TRUE;
    }
}

BOOL WINAPI XamlSystemDefaultFontForDpi(LPLOGFONT lfFont, UINT dpi)
{
    NONCLIENTMETRICS ncm = { 0 };
    ncm.cbSize = sizeof(ncm);
    if (pSystemParametersInfoForDpi)
    {
        if (!pSystemParametersInfoForDpi(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0, dpi)) return FALSE;
        *lfFont = ncm.lfMessageFont;
    }
    else
    {
        if (!SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0)) return FALSE;
        int sys_dpi = (int)XamlGetDpiForWindow(NULL);
        *lfFont = ncm.lfMessageFont;
        lfFont->lfHeight = MulDiv(lfFont->lfHeight, (int)dpi, sys_dpi);
        lfFont->lfWidth = MulDiv(lfFont->lfWidth, (int)dpi, sys_dpi);
    }
    return TRUE;
}

int WINAPI XamlGetSystemMetricsForDpi(int nIndex, UINT dpi)
{
    if (pGetSystemMetricsForDpi)
    {
        return pGetSystemMetricsForDpi(nIndex, dpi);
    }
    else
    {
        return MulDiv(GetSystemMetrics(nIndex), (int)dpi, (int)XamlGetDpiForWindow(NULL));
    }
}

UINT WINAPI XamlGetDpiForWindow(HWND hWnd)
{
    if (hWnd)
    {
        if (pGetDpiForWindow)
            return pGetDpiForWindow(hWnd);
    }
    else
    {
        if (pGetDpiForSystem)
            return pGetDpiForSystem();
    }
    HDC hDC = GetDC(hWnd);
    if (hDC)
    {
        int dpi = GetDeviceCaps(hDC, LOGPIXELSX);
        int res = ReleaseDC(hWnd, hDC);
        assert(res);
        return dpi;
    }
    else
    {
        return USER_DEFAULT_SCREEN_DPI;
    }
}
