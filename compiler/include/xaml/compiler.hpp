#ifndef XAMLC_COMPILER_HPP
#define XAMLC_COMPILER_HPP

#include <iosfwd>
#include <set>
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
        int32_t m_indent_count{ 0 };

    public:
        compiler(meta_context& ctx) : m_ctx(&ctx) {}
        ~compiler() {}

    private:
        XAMLC_API std::string xaml_cpp_compile(guid const& type, std::string_view code);

    protected:
        XAMLC_API std::ostream& write_indent(std::ostream& stream);

        XAMLC_API std::ostream& write_include(std::ostream& stream, std::string_view header);

        template <typename Container>
        std::ostream& write_includes(std::ostream& stream, Container&& headers)
        {
            for (auto h : headers)
            {
                write_include(stream, h);
            }
            return stream;
        }

        XAMLC_API std::ostream& write_begin_block(std::ostream& stream);
        XAMLC_API std::ostream& write_end_block(std::ostream& stream);

        XAMLC_API std::ostream& write_static_file_content(std::ostream& stream, std::filesystem::path const& p);

        XAMLC_API std::ostream& write_init_decl(std::ostream& stream, std::string_view ns, std::string_view name);
        XAMLC_API std::ostream& write_init_decl_with_meta(std::ostream& stream, std::string_view ns, std::string_view name);

        XAMLC_API std::ostream& write_type(std::ostream& stream, reflection_info const* type);
        XAMLC_API std::ostream& write_type(std::ostream& stream, enum_reflection_info const* type);
        XAMLC_API std::ostream& write_type(std::ostream& stream, std::string_view ns, std::string_view name);
        XAMLC_API std::ostream& write_args(std::ostream& stream, std::initializer_list<std::string_view> args);
        XAMLC_API std::ostream& write_construct(std::ostream& stream, std::string_view name, reflection_info const* type);
        XAMLC_API std::ostream& write_call(std::ostream& stream, std::string_view name, std::string_view prefix, std::string_view method, std::initializer_list<std::string_view> args);
        XAMLC_API std::ostream& write_static_call(std::ostream& stream, reflection_info const* type, std::string_view prefix, std::string_view method, std::initializer_list<std::string_view> args);

        XAMLC_API std::ostream& write_set_property(std::ostream& stream, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_set_property(std::ostream& stream, reflection_info const* type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_set_property(std::ostream& stream, reflection_info const* node_type, reflection_info const* host_type, guid const& prop_type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_property(std::ostream& stream, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_property(std::ostream& stream, reflection_info const* type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_property(std::ostream& stream, reflection_info const* node_type, reflection_info const* host_type, guid const& prop_type, std::string_view name, std::string_view prop, std::string_view value);
        XAMLC_API std::ostream& write_add_event(std::ostream& stream, xaml_node& this_node, std::string_view name, xaml_event& ev);

        XAMLC_API std::ostream& write_bind(std::ostream& stream, std::string_view target_name, std::string_view target_prop, std::string_view source_name, std::string_view source_prop);
        XAMLC_API std::ostream& write_markup(std::ostream& stream, std::string_view name, std::string_view prop, std::shared_ptr<meta_class> markup);

        XAMLC_API std::ostream& write_deserialize(std::ostream& stream, std::filesystem::path const& path);

        XAMLC_API std::ostream& compile_impl(std::ostream& stream, xaml_node& node, xaml_node& this_node, bool is_this);
        XAMLC_API std::ostream& compile_extensions(std::ostream& stream, xaml_node& node, bool is_this);

    public:
        XAMLC_API std::ostream& compile(std::ostream& stream, xaml_node& node, std::filesystem::path const& path, std::set<std::string> const& headers);
        XAMLC_API std::ostream& compile_fake(std::ostream& stream, xaml_node& node, std::filesystem::path const& path);
    };
} // namespace xaml

#endif // !XAMLC_COMPILER_HPP
