#ifndef XAML_UI_WINRT_CONTROL_H
#define XAML_UI_WINRT_CONTROL_H

#include <xaml/ui/control.h>

#include <Windows.UI.Xaml.h>

XAML_CLASS(xaml_winrt_control, { 0xb9bfb3eb, 0xccb7, 0x4682, { 0xb0, 0x84, 0x33, 0xc4, 0x85, 0xca, 0x34, 0xb5 } })

#define XAML_WINRT_CONTROL_VTBL(type)          \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(handle, type, __x_ABI_CWindows_CUI_CXaml_CIFrameworkElement**, __x_ABI_CWindows_CUI_CXaml_CIFrameworkElement*)

XAML_DECL_INTERFACE_(xaml_winrt_control, xaml_object)
{
    XAML_DECL_VTBL(xaml_winrt_control, XAML_WINRT_CONTROL_VTBL);
};

#endif // !XAML_UI_WINRT_CONTROL_H
