#ifndef XAML_UI_NATIVE_CONTROL_H
#define XAML_UI_NATIVE_CONTROL_H

#include <xaml/ui/control.h>

#ifdef XAML_UI_WINDOWS
#include <Windows.h>
#endif // XAML_UI_WINDOWS

XAML_CLASS(xaml_native_control, { 0xc02fd012, 0xc951, 0x4d34, { 0xb2, 0x40, 0x6b, 0xb8, 0x4f, 0x31, 0xd7, 0x10 } })

#define XAML_NATIVE_CONTROL_VTBL_BASE(type)                 \
    XAML_CONTROL_VTBL(type);                                \
    XAML_METHOD(draw, type, xaml_rectangle XAML_CONST_REF); \
    XAML_METHOD(size_to_fit, type);                         \
    XAML_METHOD(parent_redraw, type)

#ifdef XAML_UI_WINDOWS
#define XAML_NATIVE_CONTROL_VTBL(type)                                      \
    XAML_NATIVE_CONTROL_VTBL_BASE(type);                                    \
    XAML_METHOD(measure_string, type, xaml_string*, xaml_size, xaml_size*); \
    XAML_METHOD(wnd_proc, type, HWND, UINT, WPARAM, LPARAM, LPARAM*);       \
    XAML_METHOD(set_size_noevent, type, xaml_size XAML_CONST_REF);          \
    XAML_PROP(real_size, type, xaml_size*, xaml_size XAML_CONST_REF);       \
    XAML_PROP(real_margin, type, xaml_margin*, xaml_margin XAML_CONST_REF)
#else
#define XAML_NATIVE_CONTROL_VTBL(type) XAML_NATIVE_CONTROL_VTBL_BASE(type)
#endif // XAML_UI_WINDOWS

XAML_DECL_INTERFACE_(xaml_native_control, xaml_control)
{
    XAML_DECL_VTBL(xaml_native_control, XAML_NATIVE_CONTROL_VTBL);
};

#endif // !XAML_UI_NATIVE_CONTROL_H
