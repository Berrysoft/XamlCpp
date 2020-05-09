#include <xaml/compiler/cpp.h>
#include <xaml/event.h>
#include <xaml/internal/stream.hpp>
#include <xaml/meta/enum_info.h>

using namespace std;

struct compiler
{
    xaml_ptr<xaml_meta_context> m_ctx;
    int32_t m_indent_count{ 0 };

    compiler(xaml_ptr<xaml_meta_context> const& ctx) noexcept : m_ctx(ctx) {}

    xaml_result xaml_cpp_compile(xaml_guid const& type, xaml_ptr<xaml_string> const& code, xaml_string** ptr) noexcept;

    xaml_result write_indent(ostream& stream) noexcept;

    xaml_result write_include(ostream& stream, xaml_ptr<xaml_string> const& header) noexcept;

    xaml_result write_begin_block(ostream& stream) noexcept;
    xaml_result write_end_block(ostream& stream) noexcept;

    xaml_result write_static_file_content(ostream& stream, xaml_ptr<xaml_string> const& p) noexcept;

    xaml_result write_init_decl(ostream& stream, xaml_ptr<xaml_string> const& name) noexcept;
    xaml_result write_init_decl_with_meta(ostream& stream, xaml_ptr<xaml_string> const& name) noexcept;

    xaml_result write_type(ostream& stream, xaml_ptr<xaml_type_info> const& type) noexcept;
    xaml_result write_type(ostream& stream, xaml_ptr<xaml_enum_info> const& type) noexcept;
    xaml_result write_type(ostream& stream, xaml_ptr<xaml_string> const& name) noexcept;
    xaml_result write_args(ostream& stream, std::initializer_list<xaml_std_string_view_t> args) noexcept;
    xaml_result write_construct(ostream& stream, xaml_ptr<xaml_string> const& name, xaml_ptr<xaml_type_info> const& type) noexcept;
    xaml_result write_call(ostream& stream, xaml_ptr<xaml_string> const& name, string_view prefix, xaml_ptr<xaml_string> const& method, std::initializer_list<xaml_std_string_view_t> args) noexcept;
    xaml_result write_static_call(ostream& stream, xaml_ptr<xaml_type_info> const& type, string_view prefix, xaml_ptr<xaml_string> const& method, std::initializer_list<xaml_std_string_view_t> args) noexcept;

    xaml_result write_set_property(ostream& stream, xaml_ptr<xaml_string> const& name, xaml_ptr<xaml_string> const& prop, xaml_ptr<xaml_string> const& value) noexcept;
    xaml_result write_set_property(ostream& stream, xaml_ptr<xaml_type_info> const& type, xaml_std_string_view_t name, xaml_std_string_view_t prop, xaml_std_string_view_t value);
    xaml_result write_set_property(ostream& stream, xaml_ptr<xaml_type_info> const& node_type, xaml_ptr<xaml_type_info> const& host_type, xaml_guid const& prop_type, xaml_std_string_view_t name, xaml_std_string_view_t prop, xaml_std_string_view_t value);
    xaml_result write_add_property(ostream& stream, xaml_std_string_view_t name, xaml_std_string_view_t prop, xaml_std_string_view_t value);
    xaml_result write_add_property(ostream& stream, xaml_ptr<xaml_type_info> const& type, xaml_std_string_view_t name, xaml_std_string_view_t prop, xaml_std_string_view_t value);
    xaml_result write_add_property(ostream& stream, xaml_ptr<xaml_type_info> const& node_type, xaml_ptr<xaml_type_info> const& host_type, xaml_guid const& prop_type, xaml_std_string_view_t name, xaml_std_string_view_t prop, xaml_std_string_view_t value);
    xaml_result write_add_event(ostream& stream, xaml_node& this_node, xaml_std_string_view_t name, xaml_ptr<xaml_event> ev);

    xaml_result write_bind(ostream& stream, xaml_std_string_view_t target_name, xaml_std_string_view_t target_prop, xaml_std_string_view_t source_name, xaml_std_string_view_t source_prop);
    xaml_result write_markup(ostream& stream, xaml_std_string_view_t name, xaml_std_string_view_t prop, xaml_ptr<xaml_object> markup);

    xaml_result write_deserialize(ostream& stream, std::filesystem::path const& path);

    xaml_result compile_impl(ostream& stream, xaml_node& node, xaml_node& this_node, bool is_this);
    xaml_result compile_extensions(ostream& stream, xaml_node& node, bool is_this);

    xaml_result compile(ostream& stream, xaml_ptr<xaml_node> const& node, xaml_ptr<xaml_vector_view> const& headers);
    xaml_result compile_fake(ostream& stream, xaml_ptr<xaml_node> const& node, xaml_ptr<xaml_string> const& path);
};

static xaml_result XAML_CALL xaml_compiler_cpp_compile_impl(ostream& stream, xaml_meta_context* ctx, xaml_node* node, xaml_vector_view* headers) noexcept
{
    compiler c{ ctx };
    return c.compile(stream, node, headers);
}

static xaml_result XAML_CALL xaml_compiler_cpp_compile_fake_impl(ostream& stream, xaml_meta_context* ctx, xaml_node* node, xaml_string* path) noexcept
{
    compiler c{ ctx };
    return c.compile_fake(stream, node, path);
}

xaml_result XAML_CALL xaml_compiler_cpp_compile(FILE* file, xaml_meta_context* ctx, xaml_node* node, xaml_vector_view* headers) noexcept
{
    return call_with_file_to_stream<char>(xaml_compiler_cpp_compile_impl, file, ctx, node, headers);
}

xaml_result XAML_CALL xaml_compiler_cpp_compile_fake(FILE* file, xaml_meta_context* ctx, xaml_node* node, xaml_string* path) noexcept
{
    return call_with_file_to_stream<char>(xaml_compiler_cpp_compile_fake_impl, file, ctx, node, path);
}

xaml_result XAML_CALL xaml_compiler_cpp_compile(ostream& stream, xaml_meta_context* ctx, xaml_node* node, xaml_vector_view* headers) noexcept
{
    return xaml_compiler_cpp_compile_impl(stream, ctx, node, headers);
}

xaml_result XAML_CALL xaml_compiler_cpp_compile_fake(ostream& stream, xaml_meta_context* ctx, xaml_node* node, xaml_string* path) noexcept
{
    return xaml_compiler_cpp_compile_fake_impl(stream, ctx, node, path);
}
