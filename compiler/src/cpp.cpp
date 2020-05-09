#include <xaml/compiler/cpp.h>
#include <xaml/internal/stream.hpp>

using namespace std;

static xaml_result XAML_CALL xaml_compiler_cpp_compile_impl(basic_ostream<xaml_char_t>& stream, xaml_meta_context* ctx, xaml_node* node, xaml_vector_view* headers) noexcept
{
}

static xaml_result XAML_CALL xaml_compiler_cpp_compile_fake_impl(basic_ostream<xaml_char_t>& stream, xaml_meta_context* ctx, xaml_node* node, xaml_string* path) noexcept
{
}

xaml_result XAML_CALL xaml_compiler_cpp_compile(FILE* file, xaml_meta_context* ctx, xaml_node* node, xaml_vector_view* headers) noexcept
{
    return call_with_file_to_stream(xaml_compiler_cpp_compile_impl, file, ctx, node, headers);
}

xaml_result XAML_CALL xaml_compiler_cpp_compile_fake(FILE* file, xaml_meta_context* ctx, xaml_node* node, xaml_string* path) noexcept
{
    return call_with_file_to_stream(xaml_compiler_cpp_compile_fake_impl, file, ctx, node, path);
}

xaml_result XAML_CALL xaml_compiler_cpp_compile(basic_ostream<xaml_char_t>& stream, xaml_meta_context* ctx, xaml_node* node, xaml_vector_view* headers) noexcept
{
    return xaml_compiler_cpp_compile_impl(stream, ctx, node, headers);
}

xaml_result XAML_CALL xaml_compiler_cpp_compile_fake(basic_ostream<xaml_char_t>& stream, xaml_meta_context* ctx, xaml_node* node, xaml_string* path) noexcept
{
    return xaml_compiler_cpp_compile_fake_impl(stream, ctx, node, path);
}
