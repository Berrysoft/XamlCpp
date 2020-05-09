#ifndef XAML_PARSER_DESERIALIZER_H
#define XAML_PARSER_DESERIALIZER_H

#include <xaml/markup/markup_extension.h>
#include <xaml/meta/meta_context.h>
#include <xaml/parser/node.h>

EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_deserialize(xaml_meta_context*, xaml_node*, xaml_object**) XAML_NOEXCEPT;
EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_deserialize_inplace(xaml_meta_context*, xaml_node*, xaml_object*, xaml_guid XAML_CONST_REF) XAML_NOEXCEPT;
EXTERN_C XAML_PARSER_API xaml_result XAML_CALL xaml_parser_deserialize_markup(xaml_meta_context*, xaml_markup_node*, xaml_markup_extension**) XAML_NOEXCEPT;

#endif // !XAML_PARSER_DESERIALIZER_H
