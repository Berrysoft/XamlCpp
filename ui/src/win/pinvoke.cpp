#include <ShellScalingApi.h>
#include <mutex>
#include <vector>
#include <wil/resource.h>
#include <wil/result_macros.h>
#include <win/pinvoke.h>

using namespace std;

using pfSetProcessDPIAware = BOOL(WINAPI*)();
static pfSetProcessDPIAware pSetProcessDPIAware = nullptr;

using pfSetProcessDpiAwareness = HRESULT(WINAPI*)(PROCESS_DPI_AWARENESS);
static pfSetProcessDpiAwareness pSetProcessDpiAwareness = nullptr;

using pfSetProcessDpiAwarenessContext = BOOL(WINAPI*)(DPI_AWARENESS_CONTEXT);
static pfSetProcessDpiAwarenessContext pSetProcessDpiAwarenessContext = nullptr;

using pfEnableNonClientDpiScaling = BOOL(WINAPI*)(HWND);
static pfEnableNonClientDpiScaling pEnableNonClientDpiScaling = nullptr;

using pfSystemParametersInfoForDpi = BOOL(WINAPI*)(UINT, UINT, PVOID, UINT, UINT);
static pfSystemParametersInfoForDpi pSystemParametersInfoForDpi = nullptr;

using pfGetSystemMetricsForDpi = int(WINAPI*)(int, UINT);
static pfGetSystemMetricsForDpi pGetSystemMetricsForDpi = nullptr;

using pfGetDpiForSystem = UINT(WINAPI*)();
static pfGetDpiForSystem pGetDpiForSystem = nullptr;

using pfGetDpiForWindow = UINT(WINAPI*)(HWND);
static pfGetDpiForWindow pGetDpiForWindow = nullptr;

static vector<wil::unique_hmodule> modules{};

static void init_methods()
{
    wil::unique_hmodule user32{ LoadLibraryEx(L"User32.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32) };
    if (user32)
    {
        pSetProcessDPIAware = (pfSetProcessDPIAware)GetProcAddress(user32.get(), "SetProcessDPIAware");
        pSetProcessDpiAwarenessContext = (pfSetProcessDpiAwarenessContext)GetProcAddress(user32.get(), "SetProcessDpiAwarenessContext");
        pEnableNonClientDpiScaling = (pfEnableNonClientDpiScaling)GetProcAddress(user32.get(), "EnableNonClientDpiScaling");
        pSystemParametersInfoForDpi = (pfSystemParametersInfoForDpi)GetProcAddress(user32.get(), "SystemParametersInfoForDpi");
        pGetSystemMetricsForDpi = (pfGetSystemMetricsForDpi)GetProcAddress(user32.get(), "GetSystemMetricsForDpi");
        pGetDpiForSystem = (pfGetDpiForSystem)GetProcAddress(user32.get(), "GetDpiForSystem");
        pGetDpiForWindow = (pfGetDpiForWindow)GetProcAddress(user32.get(), "GetDpiForWindow");
        modules.push_back(std::move(user32));
    }
    wil::unique_hmodule shcore{ LoadLibraryEx(L"Shcore.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32) };
    if (shcore)
    {
        pSetProcessDpiAwareness = (pfSetProcessDpiAwareness)GetProcAddress(shcore.get(), "SetProcessDpiAwareness");
        modules.push_back(std::move(shcore));
    }
}

static once_flag init_once_flag{};

static inline void init_methods_once()
{
    call_once(init_once_flag, init_methods);
}

BOOL WINAPI XamlSetProcessBestDpiAwareness()
{
    init_methods_once();
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
    init_methods_once();
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
    init_methods_once();
    NONCLIENTMETRICS ncm{};
    ncm.cbSize = sizeof(ncm);
    if (pSystemParametersInfoForDpi)
    {
        RETURN_IF_WIN32_BOOL_FALSE(pSystemParametersInfoForDpi(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0, dpi));
        *lfFont = ncm.lfMessageFont;
    }
    else
    {
        RETURN_IF_WIN32_BOOL_FALSE(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0));
        int sys_dpi = (int)XamlGetDpiForWindow(nullptr);
        *lfFont = ncm.lfMessageFont;
        lfFont->lfHeight = MulDiv(lfFont->lfHeight, (int)dpi, sys_dpi);
        lfFont->lfWidth = MulDiv(lfFont->lfWidth, (int)dpi, sys_dpi);
    }
    return TRUE;
}

int WINAPI XamlGetSystemMetricsForDpi(int nIndex, UINT dpi)
{
    init_methods_once();
    if (pGetSystemMetricsForDpi)
    {
        return pGetSystemMetricsForDpi(nIndex, dpi);
    }
    else
    {
        return MulDiv(GetSystemMetrics(nIndex), (int)dpi, (int)XamlGetDpiForWindow(nullptr));
    }
}

UINT WINAPI XamlGetDpiForWindow(HWND hWnd)
{
    init_methods_once();
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
    auto hDC = wil::GetDC(hWnd);
    if (hDC)
    {
        return GetDeviceCaps(hDC.get(), LOGPIXELSX);
    }
    else
    {
        return USER_DEFAULT_SCREEN_DPI;
    }
}
