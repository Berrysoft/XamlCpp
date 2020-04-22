#ifndef XAML_UI_APPLICATION_H
#define XAML_UI_APPLICATION_H

#include <xaml/object.h>
#include <xaml/string.h>
#include <xaml/ui/window.h>
#include <xaml/vector.h>

typedef enum xaml_application_theme
{
    xaml_application_theme_light,
    xaml_application_theme_dark
} xaml_application_theme;

XAML_CLASS(xaml_application, { 0x842f00fd, 0xf03c, 0x4c05, { 0xb8, 0x5f, 0x18, 0x66, 0x54, 0xa3, 0x30, 0x12 } })

#define XAML_APPLICATION_VTBL(type)                       \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));            \
    XAML_METHOD(run, type, int*);                         \
    XAML_METHOD(quit, type, int);                         \
    XAML_METHOD(get_cmd_lines, type, xaml_vector_view**); \
    XAML_METHOD(get_main_window, type, xaml_window**);    \
    XAML_METHOD(get_theme, type, xaml_application_theme*)

XAML_DECL_INTERFACE_(xaml_application, xaml_object)
{
    XAML_DECL_VTBL(xaml_application, XAML_APPLICATION_VTBL);
};

EXTERN_C XAML_UI_API xaml_result xaml_application_init(xaml_application**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result xaml_application_init_with_args(int, xaml_char_t**, xaml_application**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result xaml_application_current(xaml_application**) XAML_NOEXCEPT;

#endif // !XAML_UI_APPLICATION_H
