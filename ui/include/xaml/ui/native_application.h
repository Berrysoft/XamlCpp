#ifndef XAML_UI_NATIVE_APPLICATION_H
#define XAML_UI_NATIVE_APPLICATION_H

#include <xaml/ui/application.h>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#endif // XAML_UI_WINDOWS

XAML_CLASS(xaml_native_application, { 0x14fab51b, 0x3965, 0x4845, { 0xb3, 0xa5, 0x6d, 0xc5, 0x2b, 0x5e, 0xc0, 0x7c } })

#ifdef XAML_UI_WINDOWS
#define XAML_NATIVE_APPLICATION_VTBL(type) \
    XAML_APPLICATION_VTBL(type);           \
    XAML_METHOD(get_default_font, type, UINT, HFONT*)
#else
#define XAML_NATIVE_APPLICATION_VTBL(type) XAML_APPLICATION_VTBL(type)
#endif // XAML_UI_WINDOWS

XAML_DECL_INTERFACE_(xaml_native_application, xaml_application)
{
    XAML_DECL_VTBL(xaml_native_application, XAML_NATIVE_APPLICATION_VTBL);
};

#endif // !XAML_UI_NATIVE_APPLICATION_H
