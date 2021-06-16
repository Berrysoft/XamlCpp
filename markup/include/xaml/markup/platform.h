#ifndef XAML_MARKUP_PLATFORM_H
#define XAML_MARKUP_PLATFORM_H

#include <xaml/markup/markup_extension.h>
#include <xaml/meta/meta_macros.h>

#define XAML_PLATFORM_WINDOWS U("windows")
#define XAML_PLATFORM_GTK3 U("gtk3")
#define XAML_PLATFORM_QT5 U("qt5")
#define XAML_PLATFORM_COCOA U("cocoa")

#ifdef XAML_UI_WINDOWS
    #define XAML_PLATFORM_CURRENT XAML_PLATFORM_WINDOWS
#elif defined(XAML_UI_GTK3)
    #define XAML_PLATFORM_CURRENT XAML_PLATFORM_GTK3
#elif defined(XAML_UI_QT)
    #define XAML_PLATFORM_CURRENT XAML_PLATFORM_QT5
#elif defined(XAML_UI_COCOA)
    #define XAML_PLATFORM_CURRENT XAML_PLATFORM_COCOA
#endif // XAML_UI_WINDOWS

XAML_CLASS(xaml_platform_on, { 0x89bc56ec, 0xd68e, 0x4149, { 0xbd, 0x39, 0x1d, 0x77, 0xc2, 0x51, 0xa9, 0xaf } })

#define XAML_PLATFORM_ON_VTBL(type)                         \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));              \
    XAML_PROP(platform, type, xaml_string**, xaml_string*); \
    XAML_PROP(value, type, xaml_object**, xaml_object*)

XAML_DECL_INTERFACE_(xaml_platform_on, xaml_object)
{
    XAML_DECL_VTBL(xaml_platform_on, XAML_PLATFORM_ON_VTBL);
};

EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_platform_on_new(xaml_platform_on**) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_platform_on_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_platform_on_register(xaml_meta_context*) XAML_NOEXCEPT;

XAML_CLASS(xaml_platform, { 0x4bb192fe, 0x0bd7, 0x4c84, { 0xad, 0x86, 0x95, 0xaa, 0x9b, 0xd1, 0x68, 0xaf } })

#define XAML_PLATFORM_VTBL(type)                                      \
    XAML_VTBL_INHERIT(XAML_MARKUP_EXTENSION_VTBL(type));              \
    XAML_CPROP(platform, type, xaml_platform_on*, xaml_platform_on*); \
    XAML_METHOD(add_platform_2, type, xaml_string*, xaml_object*);    \
    XAML_METHOD(remove_platform_2, type, xaml_string*)

XAML_DECL_INTERFACE_(xaml_platform, xaml_markup_extension)
{
    XAML_DECL_VTBL(xaml_platform, XAML_PLATFORM_VTBL);
};

EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_platform_new(xaml_platform**) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_platform_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_MARKUP_API xaml_result XAML_CALL xaml_platform_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_MARKUP_PLATFORM_H
