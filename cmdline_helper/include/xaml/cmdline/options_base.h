#ifndef XAML_CMDLINE_HELPER_OPTIONS_BASE_H
#define XAML_CMDLINE_HELPER_OPTIONS_BASE_H

#include <xaml/meta/meta_context.h>
#include <xaml/meta/meta_macros.h>

#ifndef XAML_CMDLINE_HELPER_API
#define XAML_CMDLINE_HELPER_API __XAML_IMPORT
#endif // !XAML_CMDLINE_HELPER_API

XAML_CLASS(xaml_cmdline_options_base, { 0x50dd3e88, 0x5d41, 0x4418, { 0x90, 0x96, 0x7f, 0xd8, 0xb7, 0x76, 0xf9, 0xca } })

#define XAML_CMDLINE_OPTIONS_BASE_VTBL(type)   \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_PROP(help, type, bool*, bool);        \
    XAML_PROP(verbose, type, bool*, bool);     \
    XAML_PROP(debug, type, bool*, bool);       \
    XAML_PROP(no_logo, type, bool*, bool)

XAML_DECL_INTERFACE_(xaml_cmdline_options_base, xaml_object)
{
    XAML_DECL_VTBL(xaml_cmdline_options_base, XAML_CMDLINE_OPTIONS_BASE_VTBL);
};

EXTERN_C XAML_CMDLINE_HELPER_API xaml_result XAML_CALL xaml_cmdline_options_base_members(xaml_type_info_registration*) XAML_NOEXCEPT;
EXTERN_C XAML_CMDLINE_HELPER_API xaml_result XAML_CALL xaml_cmdline_options_base_register(xaml_meta_context*) XAML_NOEXCEPT;

EXTERN_C XAML_CMDLINE_HELPER_API xaml_result XAML_CALL xaml_cmdline_parse_and_print(xaml_meta_context*, xaml_guid XAML_CONST_REF, int, xaml_char_t**, void**) XAML_NOEXCEPT;

#ifdef __cplusplus
template <typename T>
xaml_result XAML_CALL xaml_cmdline_parse_and_print(xaml_meta_context* ctx, int argc, xaml_char_t** argv, T** ptr) noexcept
{
    return xaml_cmdline_parse_and_print(ctx, xaml_type_guid_v<T>, argc, argv, (void**)ptr);
}
#endif // __cplusplus

#endif // !XAML_CMDLINE_HELPER_OPTIONS_BASE_H
