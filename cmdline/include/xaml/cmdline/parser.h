#ifndef XAML_CMDLINE_PARSER_H
#define XAML_CMDLINE_PARSER_H

#include <xaml/meta/collection_property_info.h>
#include <xaml/meta/meta_context.h>
#include <xaml/meta/meta_macros.h>
#include <xaml/meta/property_info.h>
#include <xaml/meta/type_info.h>
#include <xaml/string.h>
#include <xaml/vector.h>

EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_register(xaml_meta_context*) XAML_NOEXCEPT;

XAML_CLASS(xaml_cmdline_options, { 0x5f579fe2, 0xde22, 0x4dee, { 0x95, 0xe0, 0xaf, 0xc5, 0x4d, 0xe8, 0x6f, 0x62 } })

// properties: xaml_map<xaml_property_info, xaml_string>
// collection_properties: xaml_map<xaml_string, xaml_key_value_pair<xaml_collection_property_info, xaml_vector<xaml_string>>>

#define XAML_CMDLINE_OPTIONS_VTBL(type)                 \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));          \
    XAML_PROP(properties, type, xaml_map**, xaml_map*); \
    XAML_PROP(collection_properties, type, xaml_map**, xaml_map*)

XAML_DECL_INTERFACE_(xaml_cmdline_options, xaml_object)
{
    XAML_DECL_VTBL(xaml_cmdline_options, XAML_CMDLINE_OPTIONS_VTBL);
};

EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_parse(xaml_type_info*, xaml_vector_view*, xaml_cmdline_options**) XAML_NOEXCEPT;
EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_parse_argv(xaml_type_info*, int, char**, xaml_cmdline_options**) XAML_NOEXCEPT;

#endif // !XAML_CMDLINE_PARSER_H
