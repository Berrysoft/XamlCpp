#ifndef XAML_UI_FILEBOX_H
#define XAML_UI_FILEBOX_H

#include <xaml/ui/window.h>

typedef struct xaml_filebox_filter xaml_filebox_filter;

struct xaml_filebox_filter
{
    xaml_char_t const* name;
    xaml_char_t const* pattern;
};

XAML_TYPE(xaml_filebox_filter, { 0xff79e816, 0x280b, 0x4e03, { 0x99, 0x12, 0xba, 0x3c, 0x1a, 0x74, 0xcb, 0x94 } })

XAML_CLASS(xaml_filebox, { 0xeed5f78a, 0x5be0, 0x4294, { 0xb9, 0x93, 0xd0, 0xcc, 0xf9, 0xd2, 0x10, 0xd9 } })

#define XAML_FILEBOX_VTBL(type)                                      \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                       \
    XAML_PROP(title, type, xaml_string**, xaml_string*);             \
    XAML_PROP(filename, type, xaml_string**, xaml_string*);          \
    XAML_PROP(filters, type, xaml_vector_view**, xaml_vector_view*); \
    XAML_METHOD(get_result, type, xaml_string**);                    \
    XAML_METHOD(show, type, xaml_window*)

XAML_DECL_INTERFACE_(xaml_filebox, xaml_object)
{
    XAML_DECL_VTBL(xaml_filebox, XAML_FILEBOX_VTBL);
};

XAML_CLASS(xaml_open_filebox, { 0xd04938db, 0x5363, 0x468d, { 0x84, 0x68, 0x7c, 0x30, 0x39, 0x1a, 0xc2, 0x9f } })

#define XAML_OPEN_FILEBOX_VTBL(type)            \
    XAML_VTBL_INHERIT(XAML_FILEBOX_VTBL(type)); \
    XAML_PROP(multiple, type, bool*, bool);     \
    XAML_METHOD(get_results, type, xaml_vector_view**);

XAML_DECL_INTERFACE_(xaml_open_filebox, xaml_filebox)
{
    XAML_DECL_VTBL(xaml_open_filebox, XAML_OPEN_FILEBOX_VTBL);
};

EXTERN_C XAML_UI_API xaml_result xaml_open_filebox_new(xaml_open_filebox**) XAML_NOEXCEPT;

XAML_CLASS(xaml_save_filebox, { 0x057ba594, 0xf0c4, 0x4ddd, { 0x9f, 0xc7, 0x76, 0x7d, 0x36, 0x48, 0xd3, 0x3a } })

#define XAML_SAVE_FILEBOX_VTBL(type) XAML_VTBL_INHERIT(XAML_FILEBOX_VTBL(type))

XAML_DECL_INTERFACE_(xaml_save_filebox, xaml_filebox)
{
    XAML_DECL_VTBL(xaml_save_filebox, XAML_SAVE_FILEBOX_VTBL);
};

EXTERN_C XAML_UI_API xaml_result xaml_save_filebox_new(xaml_save_filebox**) XAML_NOEXCEPT;

#endif // !XAML_UI_FILEBOX_H
