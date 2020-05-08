#ifndef XAML_COMPILER_CPP_H
#define XAML_COMPILER_CPP_H

#include <xaml/meta/meta_context.h>
#include <xaml/parser/node.h>

EXTERN_C XAML_COMPILER_API xaml_result XAML_CALL xaml_compiler_cpp_compile(xaml_meta_context*, xaml_node*, xaml_vector_view*) XAML_NOEXCEPT;
EXTERN_C XAML_COMPILER_API xaml_result XAML_CALL xaml_compiler_cpp_compile_fake(xaml_meta_context*, xaml_node*, xaml_string*) XAML_NOEXCEPT;

#endif // !XAML_COMPILER_CPP_H
