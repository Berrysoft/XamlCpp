#ifndef XAML_UI_WINRT_WINDOW_H
#define XAML_UI_WINRT_WINDOW_H

#include <xaml/ui/winrt/control.h>

XAML_CLASS(xaml_winrt_window, { 0x5cfe3e5b, 0x9140, 0x4314, { 0xa7, 0x8b, 0xe5, 0x45, 0x69, 0x29, 0xe2, 0xa7 } })

#define XAML_WINRT_WINDOW_VTBL(type) XAML_VTBL_INHERIT(XAML_WINRT_CONTROL_VTBL(type))

XAML_DECL_INTERFACE_(xaml_winrt_window, xaml_winrt_control)
{
    XAML_DECL_VTBL(xaml_winrt_window, XAML_WINRT_WINDOW_VTBL);
};

#endif // !XAML_UI_WINRT_WINDOW_H
