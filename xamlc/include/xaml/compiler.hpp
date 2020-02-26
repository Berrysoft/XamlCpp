#ifndef XAMLC_COMPILER_HPP
#define XAMLC_COMPILER_HPP

#include <iosfwd>
#include <unordered_map>
#include <vector>
#include <xaml/meta/meta.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/xaml_node.hpp>

namespace xaml
{
    class compiler
    {
    private:
        meta_context* m_ctx;
        size_t m_indent_count{ 0 };

    public:
        compiler(meta_context& ctx) : m_ctx(&ctx) {}
        ~compiler() {}

    private:
        XAMLC_API std::string xaml_cpp_compile(std::type_index type, std::string_view code);

    protected:
        XAMLC_API std::ostream& write_indent(std::ostream& stream);

        XAMLC_API std::ostream& write_include(std::ostream& stream, std::string_view header);
        XAMLC_API std::ostream& write_includes(std::ostream& stream, std::vector<std::string_view> const& headers);

        XAMLC_API std::ostream& write_begin_block(std::ostream& stream);
        XAMLC_API std::ostream& write_end_block(std::ostream& stream);

        XAMLC_API std::ostream& write_init_decl(std::ostream& stream, std::string_view ns, std::string_view name);
        XAMLC_API std::ostream& write_init_decl_with_meta(std::ostream& stream, std::string_view ns, std::string_view name);

        XAMLC_API std::ostream& write_type(std::ostream& stream, std::type_index type);
        XAMLC_API std::ostream& write_type(std::ostream& stream, std::string_view ns, std::string_view name);
        XAMLC_API std::ostream& write_args(std::ostream& stream, std::initializer_list<std::string_view> args);
        XAMLC_API std::ostream& write_construct(std::ostream& stream, std::string_view name, std::type_index type);
        XAMLC_API std::ostream& write_call(std::ostream& stream, std::string_view name, std::string_view prefix, std::string_view method, std::initializer_list<std::string_view> args);
        XAMLC_API std::ostream& write_static_call(std::ostream& stream, std::type_index type, std::string_view prefix, std::string_view method, std::initializer_list<std::string_view> args);

        XAMLC_API std::ostream& write_set_property(std::ostream& stream, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_set_property(std::ostream& stream, std::type_index type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_set_property(std::ostream& stream, std::type_index node_type, std::type_index host_type, std::type_index prop_type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_property(std::ostream& stream, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_property(std::ostream& stream, std::type_index type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_property(std::ostream& stream, std::type_index node_type, std::type_index host_type, std::type_index prop_type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_event(std::ostream& stream, xaml_node& this_node, std::string_view name, xaml_event& ev);

        XAMLC_API std::ostream& write_markup(std::ostream& stream, std::string_view name, std::string_view prop, std::shared_ptr<meta_class> markup);

        XAMLC_API std::ostream& write_deserialize(std::ostream& stream, std::string_view path);

        XAMLC_API std::ostream& compile_impl(std::ostream& stream, xaml_node& node, xaml_node& this_node, bool is_this);
        XAMLC_API std::ostream& compile_extensions(std::ostream& stream, xaml_node& node, bool is_this);

    public:
        XAMLC_API std::ostream& compile(std::ostream& stream, xaml_node& node);
        XAMLC_API std::ostream& compile_fake(std::ostream& stream, xaml_node& node, std::string_view path);
    };
} // namespace xaml

#endif // !XAMLC_COMPILER_HPP
