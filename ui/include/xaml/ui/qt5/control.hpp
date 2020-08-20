#ifndef XAML_UI_QT5_CONTROL_H
#define XAML_UI_QT5_CONTROL_H

#include <QSharedPointer>
#include <QWidget>
#include <xaml/meta/meta_macros.h>

XAML_CLASS(xaml_qt5_control, { 0x0cafa395, 0x4050, 0x4b84, { 0x8f, 0xeb, 0x02, 0xaa, 0xa1, 0x27, 0x21, 0x7b } })

#define XAML_QT5_CONTROL_VTBL(type)                          \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));               \
    XAML_METHOD_(QSharedPointer<QWidget>, get_handle, type); \
    XAML_METHOD_(void, set_handle, type, QSharedPointer<QWidget> XAML_CONST_REF)

XAML_DECL_INTERFACE_(xaml_qt5_control, xaml_object)
{
    XAML_DECL_VTBL(xaml_qt5_control, XAML_QT5_CONTROL_VTBL);
};

#endif // !XAML_UI_QT5_CONTROL_H
