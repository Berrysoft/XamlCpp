#include <Windows.h>
#include <xaml/result_win32.h>
#include <xaml/ui/monitor.h>
#include <xaml/ui/native_drawing.hpp>

using namespace std;

struct xaml_monitor_get_args
{
    xaml_vector* vec;
    xaml_result res;
};

static xaml_result xaml_monitor_enum(HMONITOR m, xaml_vector* vec)
{
    MONITORINFO info = {};
    info.cbSize = sizeof(MONITORINFO);
    XAML_RETURN_IF_WIN32_BOOL_FALSE(GetMonitorInfo(m, &info));
    xaml_ptr<xaml_object> value;
    XAML_RETURN_IF_FAILED(xaml_box_value<xaml_monitor>({ xaml_from_native(info.rcMonitor), xaml_from_native(info.rcWork) }));
    return vec->append(value.get());
}

static BOOL CALLBACK MonitorEnum(HMONITOR m, HDC, LPRECT, LPARAM arg)
{
    xaml_monitor_get_args& args = *(xaml_monitor_get_args*)arg;
    xaml_result hr = xaml_monitor_enum(m, args.vec);
    if (XAML_SUCCESS(hr))
    {
        return TRUE;
    }
    else
    {
        args.res = hr;
        return FALSE;
    }
}

xaml_result xaml_monitor_get_all(xaml_vector_view** ptr) noexcept
{
    xaml_ptr<xaml_vector> result;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&result));
    xaml_monitor_get_args args{ result.get(), XAML_S_OK };
    XAML_RETURN_IF_WIN32_BOOL_FALSE(EnumDisplayMonitors(nullptr, nullptr, MonitorEnum, (LPARAM)&args));
    if (XAML_SUCCESS(args.res))
    {
        result->query(ptr);
        return XAML_S_OK;
    }
    else
    {
        return args.res;
    }
}
