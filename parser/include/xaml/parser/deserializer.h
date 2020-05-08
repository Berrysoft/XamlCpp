#ifndef XAML_PARSER_DESERIALIZER_H
#define XAML_PARSER_DESERIALIZER_H

#include <xaml/parser/node.h>

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_deserialize(xaml_node*, xaml_object**) XAML_NOEXCEPT;
EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_deserialize_inplace(xaml_node*, xaml_object*) XAML_NOEXCEPT;
EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_deserialize_markup(xaml_markup_node*, xaml_object**) XAML_NOEXCEPT;

#endif // !XAML_PARSER_DESERIALIZER_H
