#ifndef XAML_UI_QT5_MENU_BAR_H
#define XAML_UI_QT5_MENU_BAR_H

#include <QMenuBar>
#include <xaml/meta/meta_macros.h>
#include <xaml/object.h>

XAML_CLASS(xaml_qt5_menu_bar, { 0xe7b1ca82, 0xa1cd, 0x4fb7, { 0x84, 0x05, 0x80, 0xf1, 0xc5, 0x8d, 0x94, 0x87 } })

#define XAML_QT5_MENU_BAR_VTBL(type)           \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(handle, type, QMenuBar**, QMenuBar*)

XAML_DECL_INTERFACE_(xaml_qt5_menu_bar, xaml_object)
{
    XAML_DECL_VTBL(xaml_qt5_menu_bar, XAML_QT5_MENU_BAR_VTBL);
};

#endif // !XAML_UI_QT5_MENU_BAR_H
