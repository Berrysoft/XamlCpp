#ifndef XAML_PARSER_PARSER_H
#define XAML_PARSER_PARSER_H

#include <xaml/meta/meta_context.h>
#include <xaml/parser/node.h>

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_register(xaml_meta_context*) XAML_NOEXCEPT;

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_string(xaml_meta_context*, char const*, xaml_node**, xaml_vector_view**) XAML_NOEXCEPT;
EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_stream(xaml_meta_context*, XAML_CSTD FILE*, xaml_node**, xaml_vector_view**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_stream(xaml_meta_context*, std::istream& stream, xaml_node**, xaml_vector_view**) noexcept;
#endif // __cplusplus

#endif // !XAML_PARSER_PARSER_H
