#include <ShellScalingApi.h>
#include <Windows.h>
#include <wil/result_macros.h>
#include <xaml/ui/screen.hpp>

using namespace std;

namespace xaml
{
    static BOOL CALLBACK MonitorEnum(HMONITOR m, HDC, LPRECT, LPARAM arg)
    {
        auto& callback = *(vector<monitor>*)arg;
        MONITORINFO info = {};
        info.cbSize = sizeof(MONITORINFO);
        THROW_IF_WIN32_BOOL_FALSE(GetMonitorInfo(m, &info));
        UINT dpix, dpiy;
        THROW_IF_FAILED(GetDpiForMonitor(m, MDT_EFFECTIVE_DPI, &dpix, &dpiy));
        callback.push_back({ from_native(info.rcMonitor), from_native(info.rcWork), (double)dpix });
        return TRUE;
    }

    vector<monitor> get_monitors()
    {
        vector<monitor> ms;
        THROW_IF_WIN32_BOOL_FALSE(EnumDisplayMonitors(NULL, NULL, MonitorEnum, (LPARAM)&ms));
        return ms;
    }
} // namespace xaml
