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

#ifndef xaml_key_value_pair_2__xaml_property_info__xaml_string_defined
    #define xaml_key_value_pair_2__xaml_property_info__xaml_string_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_O(xaml_property_info), XAML_T_O(xaml_string))
#endif // !xaml_key_value_pair_2__xaml_property_info__xaml_string_defined

#ifndef xaml_enumerator_1__xaml_key_value_pair_2__xaml_property_info__xaml_string_defined
    #define xaml_enumerator_1__xaml_key_value_pair_2__xaml_property_info__xaml_string_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_key_value_pair_2__xaml_property_info__xaml_string))
#endif // !xaml_enumerator_1__xaml_key_value_pair_2__xaml_property_info__xaml_string_defined

#ifndef xaml_map_2__xaml_property_info__xaml_string_defined
    #define xaml_map_2__xaml_property_info__xaml_string_defined
XAML_MAP_2_TYPE(XAML_T_O(xaml_property_info), XAML_T_O(xaml_string))
#endif // !xaml_map_2__xaml_property_info__xaml_string_defined

#ifndef xaml_enumerator_1__xaml_string_defined
    #define xaml_enumerator_1__xaml_string_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_enumerator_1__xaml_string_defined

#ifndef xaml_vector_view_1__xaml_string_defined
    #define xaml_vector_view_1__xaml_string_defined
XAML_VECTOR_VIEW_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_vector_view_1__xaml_string_defined

#ifndef xaml_vector_1__xaml_string_defined
    #define xaml_vector_1__xaml_string_defined
XAML_VECTOR_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_vector_1__xaml_string_defined

#ifndef xaml_key_value_pair_2__xaml_collection_proeprty_info__xaml_vector_1__xaml_string_defined
    #define xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_O(xaml_collection_property_info), XAML_T_O(xaml_vector_1__xaml_string))
#endif // !xaml_key_value_pair_2__xaml_collection_proeprty_info__xaml_vector_1__xaml_string_defined

#ifndef xaml_key_value_pair_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string_defined
    #define xaml_key_value_pair_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string_defined
XAML_KEY_VALUE_PAIR_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string))
#endif // !xaml_key_value_pair_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string_defined

#ifndef xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string_defined
    #define xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_key_value_pair_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string))
#endif // !xaml_enumerator_1__xaml_key_value_pair_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string_defined

#ifndef xaml_map_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string_defined
    #define xaml_map_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string_defined
XAML_MAP_2_TYPE(XAML_T_O(xaml_string), XAML_T_O(xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string))
#endif // !xaml_map_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string_defined

XAML_CLASS(xaml_cmdline_options, { 0x5f579fe2, 0xde22, 0x4dee, { 0x95, 0xe0, 0xaf, 0xc5, 0x4d, 0xe8, 0x6f, 0x62 } })

#define XAML_CMDLINE_OPTIONS_VTBL(type)                                                                                                 \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                                                                                          \
    XAML_PROP(properties, type, XAML_MAP_2_NAME(xaml_property_info, xaml_string)**, XAML_MAP_2_NAME(xaml_property_info, xaml_string)*); \
    XAML_PROP(collection_properties, type, XAML_MAP_2_NAME(xaml_string, XAML_KEY_VALUE_PAIR_2_NAME(xaml_collection_property_info, XAML_VECTOR_1_NAME(xaml_string)))**, XAML_MAP_2_NAME(xaml_string, XAML_KEY_VALUE_PAIR_2_NAME(xaml_collection_property_info, XAML_VECTOR_1_NAME(xaml_string)))*)

XAML_DECL_INTERFACE_(xaml_cmdline_options, xaml_object)
{
    XAML_DECL_VTBL(xaml_cmdline_options, XAML_CMDLINE_OPTIONS_VTBL);
};

EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_parse(xaml_type_info*, XAML_VECTOR_VIEW_1_NAME(xaml_string) *, xaml_cmdline_options**) XAML_NOEXCEPT;
EXTERN_C XAML_CMDLINE_API xaml_result XAML_CALL xaml_cmdline_parse_argv(xaml_type_info*, int, char**, xaml_cmdline_options**) XAML_NOEXCEPT;

#endif // !XAML_CMDLINE_PARSER_H
