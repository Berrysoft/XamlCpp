#include <Windows.h>
#include <wil/result_macros.h>
#include <xaml/ui/screen.hpp>

using namespace std;

namespace xaml
{
    unsigned int screen_dpi()
    {
        return GetDpiForWindow(GetDesktopWindow());
    }

    static constexpr rectangle get_rect(RECT const& r) { return { r.left, r.top, r.right - r.left, r.bottom - r.top }; }

    static BOOL CALLBACK MonitorEnum(HMONITOR m, HDC, LPRECT, LPARAM arg)
    {
        auto& callback = *(vector<monitor>*)arg;
        MONITORINFO info = {};
        info.cbSize = sizeof(MONITORINFO);
        THROW_IF_WIN32_BOOL_FALSE(GetMonitorInfo(m, &info));
        callback.push_back({ get_rect(info.rcMonitor), get_rect(info.rcWork) });
        return TRUE;
    }

    vector<monitor> get_monitors()
    {
        vector<monitor> ms;
        THROW_IF_WIN32_BOOL_FALSE(EnumDisplayMonitors(NULL, NULL, MonitorEnum, (LPARAM)&ms));
        return ms;
    }
} // namespace xaml
