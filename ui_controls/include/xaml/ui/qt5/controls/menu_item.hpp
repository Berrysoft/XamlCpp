#ifndef XAML_UI_CONTROLS_QT5_MENU_ITEM_HPP
#define XAML_UI_CONTROLS_QT5_MENU_ITEM_HPP

#include <QAction>
#include <xaml/meta/meta_macros.h>

XAML_CLASS(xaml_qt5_menu_item, { 0xd6e77447, 0xe8cd, 0x475b, { 0x91, 0x8e, 0x31, 0xaa, 0x62, 0x24, 0xaf, 0xf4 } })

#define XAML_QT5_MENU_ITEM_VTBL(type)          \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(action, type, QAction**, QAction*);

XAML_DECL_INTERFACE_(xaml_qt5_menu_item, xaml_object)
{
    XAML_DECL_VTBL(xaml_qt5_menu_item, XAML_QT5_MENU_ITEM_VTBL);
};

#endif // !XAML_UI_CONTROLS_QT5_MENU_ITEM_HPP
