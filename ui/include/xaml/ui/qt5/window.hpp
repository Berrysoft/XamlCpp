#ifndef XAML_UI_QT_WINDOW_H
#define XAML_UI_QT_WINDOW_H

#include <xaml/ui/qt5/control.hpp>
#include <xaml/ui/window.h>

XAML_CLASS(xaml_qt5_window, { 0x88275696, 0xe2e3, 0x4631, { 0xb1, 0x67, 0x69, 0x4a, 0x0e, 0x36, 0xf2, 0x93 } })

#define XAML_QT5_WINDOW_VTBL(type) XAML_VTBL_INHERIT(XAML_QT5_CONTROL_VTBL(type))

XAML_DECL_INTERFACE_(xaml_qt5_window, xaml_qt5_control)
{
    XAML_DECL_VTBL(xaml_qt5_window, XAML_QT5_WINDOW_VTBL);
};

#endif // !XAML_UI_QT_WINDOW_H
