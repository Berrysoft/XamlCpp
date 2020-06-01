#ifndef XAML_PARSER_PARSER_H
#define XAML_PARSER_PARSER_H

#include <xaml/buffer.h>
#include <xaml/meta/meta_context.h>
#include <xaml/parser/node.h>

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_string(xaml_meta_context*, xaml_string*, xaml_node**, xaml_vector_view**) XAML_NOEXCEPT;
EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_buffer(xaml_meta_context*, xaml_buffer*, xaml_node**, xaml_vector_view**) XAML_NOEXCEPT;

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_stream(xaml_meta_context*, XAML_STD FILE*, xaml_node**, xaml_vector_view**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_PARSER_API xaml_result XAML_CALL xaml_parser_parse_stream(xaml_meta_context*, std::istream&, xaml_node**, xaml_vector_view**) noexcept;
#endif // __cplusplus

#endif // !XAML_PARSER_PARSER_H
