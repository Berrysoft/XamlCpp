#ifndef XAML_UI_WIN_CONTROL_H
#define XAML_UI_WIN_CONTROL_H

#include <Windows.h>
#include <xaml/object.h>
#include <xaml/ui/drawing.h>

typedef struct xaml_win32_window_message
{
    HWND hWnd;
    UINT msg;
    WPARAM wParam;
    LPARAM lParam;
} xaml_win32_window_message;

typedef struct xaml_win32_window_create_params
{
    xaml_char_t const* class_name;
    xaml_char_t const* window_name;
    DWORD style;
    DWORD ex_style;
    int x;
    int y;
    int width;
    int height;
    xaml_control* parent;
} xaml_win32_window_create_params;

XAML_CLASS(xaml_win32_control, { 0x5aa117c2, 0x59f0, 0x43d6, { 0x94, 0x4c, 0x41, 0x2b, 0x52, 0xa9, 0x35, 0x3b } })

#define XAML_WIN32_CONTROL_VTBL(type)                                               \
    XAML_PROP(handle, type, HWND*, HWND);                                           \
    XAML_METHOD(wnd_proc, type, xaml_win32_window_message XAML_CONST_REF, LPARAM*); \
    XAML_PROP(real_size, type, xaml_size*, xaml_size XAML_CONST_REF);               \
    XAML_PROP(real_margin, type, xaml_margin*, xaml_margin XAML_CONST_REF)

XAML_DECL_INTERFACE_(xaml_win32_control, xaml_object)
{
    XAML_DECL_VTBL(xaml_win32_control, XAML_WIN32_CONTROL_VTBL);
};

#endif // !XAML_UI_WIN_CONTROL_H
