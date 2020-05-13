#ifndef XAML_UI_WIN_FONT_PROVIDER_H
#define XAML_UI_WIN_FONT_PROVIDER_H

#include <Windows.h>
#include <xaml/object.h>

XAML_CLASS(xaml_win32_font_provider, { 0x9cdb84b9, 0x792e, 0x41eb, { 0x8d, 0xf5, 0x0c, 0x20, 0x4d, 0x96, 0x98, 0x36 } })

#define XAML_WIN32_FONT_PROVIDER_VTBL(type)    \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(get_default_font, type, UINT, HFONT*)

XAML_DECL_INTERFACE_(xaml_win32_font_provider, xaml_object)
{
    XAML_DECL_VTBL(xaml_win32_font_provider, XAML_WIN32_FONT_PROVIDER_VTBL);
};

#endif // !XAML_UI_WIN_FONT_PROVIDER_H
