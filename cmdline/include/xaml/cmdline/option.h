#ifndef XAML_CMDLINE_OPTION_H
#define XAML_CMDLINE_OPTION_H

#ifdef __cplusplus
#include <cstdio>
#include <ostream>
#else
#include <stdio.h>
#endif // __cplusplus

#include <xaml/meta/meta_macros.h>
#include <xaml/object.h>
#include <xaml/string.h>

XAML_CLASS(xaml_cmdline_option_entry, { 0x09a9e799, 0x5c21, 0x4630, { 0x8f, 0x87, 0x42, 0xbf, 0x4a, 0x61, 0x5d, 0xf5 } })

#define XAML_CMDLINE_OPTION_ENTRY_VTBL(type)                \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));              \
    XAML_PROP(short_arg, type, xaml_char_t*, xaml_char_t);  \
    XAML_PROP(long_arg, type, xaml_string**, xaml_string*); \
    XAML_PROP(help_text, type, xaml_string**, xaml_string*)

XAML_DECL_INTERFACE_(xaml_cmdline_option_entry, xaml_object)
{
    XAML_DECL_VTBL(xaml_cmdline_option_entry, XAML_CMDLINE_OPTION_ENTRY_VTBL);
};

XAML_CLASS(xaml_cmdline_option, { 0x240bec4e, 0xefc2, 0x45ed, { 0xa5, 0x01, 0x04, 0xc6, 0x44, 0xf3, 0xf7, 0x0d } })

#define XAML_CMDLINE_OPTION_VTBL(type)                             \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                     \
    XAML_METHOD(find_short_arg, type, xaml_char_t, xaml_string**); \
    XAML_METHOD(find_long_arg, type, xaml_string*, xaml_string**); \
    XAML_METHOD(get_default_property, type, xaml_string**);        \
    XAML_METHOD(get_entries, type, xaml_map_view**);               \
    XAML_METHOD(add_arg, type, xaml_char_t, xaml_string*, xaml_string*, xaml_string*)

XAML_DECL_INTERFACE_(xaml_cmdline_option, xaml_object)
{
    XAML_DECL_VTBL(xaml_cmdline_option, XAML_CMDLINE_OPTION_VTBL);
};

EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_option_new(xaml_cmdline_option**) XAML_NOEXCEPT;
EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_option_print(XAML_CSTD FILE*, xaml_cmdline_option*) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_option_print(std::basic_ostream<xaml_char_t>&, xaml_cmdline_option*) noexcept;
#endif // __cplusplus

#endif // !XAML_CMDLINE_OPTION_H
