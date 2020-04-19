#ifndef XAML_UI_APPLICATION_H
#define XAML_UI_APPLICATION_H

#include <xaml/object.h>
#include <xaml/string.h>
#include <xaml/vector.h>

typedef enum xaml_application_theme
{
    xaml_application_theme_light,
    xaml_application_theme_dark
} xaml_application_theme;

XAML_CLASS(xaml_application, { 0x842f00fd, 0xf03c, 0x4c05, { 0xb8, 0x5f, 0x18, 0x66, 0x54, 0xa3, 0x30, 0x12 } })

typedef struct xaml_window xaml_window;

#ifdef __cplusplus
struct XAML_NOVTBL xaml_application : xaml_object
{
    virtual xaml_result XAML_CALL run(int*) noexcept = 0;
    virtual xaml_result XAML_CALL quit(int) noexcept = 0;
    virtual xaml_result XAML_CALL get_cmd_lines(xaml_vector_view**) noexcept = 0;
    virtual xaml_result XAML_CALL get_main_window(xaml_window**) noexcept = 0;
    virtual xaml_result XAML_CALL get_theme(xaml_application_theme*) noexcept = 0;
};
#else

#endif // __cplusplus

EXTERN_C XAML_UI_API xaml_result xaml_application_init(xaml_application**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result xaml_application_init_with_args(int, xaml_char_t**, xaml_application**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result xaml_application_current(xaml_application**) XAML_NOEXCEPT;

#endif // !XAML_UI_APPLICATION_H
