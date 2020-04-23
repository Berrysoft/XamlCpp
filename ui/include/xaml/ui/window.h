#ifndef XAML_UI_WINDOW_H
#define XAML_UI_WINDOW_H

#include <xaml/ui/container.h>

XAML_CLASS(xaml_window, { 0xf65be476, 0xbef3, 0x4d90, { 0xa6, 0xcd, 0x89, 0x33, 0x38, 0x9c, 0x2c, 0x53 } })

#define XAML_WINDOW_VTBL(type)                                         \
    XAML_VTBL_INHERIT(XAML_CONTAINER_VTBL(type));                      \
    XAML_METHOD(show, type);                                           \
    XAML_METHOD(close, type);                                          \
    XAML_METHOD(hide, type);                                           \
    XAML_PROP(is_resizable, type, bool*, bool);                        \
    XAML_EVENT(is_resizable_changed, type);                            \
    XAML_PROP(location, type, xaml_point*, xaml_point XAML_CONST_REF); \
    XAML_EVENT(location_changed, type);                                \
    XAML_PROP(x, type, double*, double);                               \
    XAML_PROP(y, type, double*, double);                               \
    XAML_PROP(title, type, xaml_string**, xaml_string);                \
    XAML_EVENT(title_changed, type);                                   \
    XAML_EVENT(closing, type);                                         \
    XAML_METHOD(get_client_region, type, xaml_rectangle*);             \
    XAML_METHOD(get_dpi, type, double*)

XAML_DECL_INTERFACE_(xaml_window, xaml_container)
{
    XAML_DECL_VTBL(xaml_window, XAML_WINDOW_VTBL);
};

EXTERN_C XAML_UI_API xaml_result xaml_window_new(xaml_window**) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result xaml_window_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_UI_API xaml_result xaml_window_register(xaml_meta_context*) XAML_NOEXCEPT;

#endif // !XAML_UI_WINDOW_H
