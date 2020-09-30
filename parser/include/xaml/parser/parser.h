#ifndef XAML_PARSER_PARSER_H
#define XAML_PARSER_PARSER_H

#include <xaml/buffer.h>
#include <xaml/meta/meta_context.h>
#include <xaml/parser/node.h>

#ifndef xaml_enumerator_1__xaml_string_defined
    #define xaml_enumerator_1__xaml_string_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_enumerator_1__xaml_string_defined

#ifndef xaml_vector_view_1__xaml_string_defined
    #define xaml_vector_view_1__xaml_string_defined
XAML_VECTOR_VIEW_1_TYPE(XAML_T_O(xaml_string))
#endif // !xaml_vector_view_1__xaml_string_defined

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_string(xaml_meta_context*, xaml_string*, xaml_node**, XAML_VECTOR_VIEW_1_NAME(xaml_string) **) XAML_NOEXCEPT;
EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_buffer(xaml_meta_context*, xaml_buffer*, xaml_node**, XAML_VECTOR_VIEW_1_NAME(xaml_string) **) XAML_NOEXCEPT;

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_stream(xaml_meta_context*, XAML_STD FILE*, xaml_node**, XAML_VECTOR_VIEW_1_NAME(xaml_string) **) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_stream(xaml_meta_context*, std::istream&, xaml_node**, XAML_VECTOR_VIEW_1_NAME(xaml_string) **) noexcept;
#endif // __cplusplus

#endif // !XAML_PARSER_PARSER_H
