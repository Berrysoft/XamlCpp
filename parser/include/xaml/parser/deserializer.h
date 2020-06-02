#ifndef XAML_PARSER_DESERIALIZER_H
#define XAML_PARSER_DESERIALIZER_H

#include <xaml/markup/markup_extension.h>
#include <xaml/meta/meta_context.h>
#include <xaml/parser/node.h>

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_deserialize(xaml_meta_context*, xaml_node*, xaml_object**) XAML_NOEXCEPT;
EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_deserialize_inplace(xaml_meta_context*, xaml_node*, xaml_object*) XAML_NOEXCEPT;

#endif // !XAML_PARSER_DESERIALIZER_H
