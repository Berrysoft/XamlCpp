#ifndef XAMLC_COMPILER_HPP
#define XAMLC_COMPILER_HPP

#include <iosfwd>
#include <xaml/parser.hpp>

namespace xaml
{
    class compiler
    {
    private:
        parser reader;

    public:
        constexpr bool is_open() const noexcept { return reader.is_open(); }
        void open(std::string_view file) { reader.open(file); }

        compiler() : reader() {}
        compiler(std::string_view file) : compiler() { open(file); }
        ~compiler() {}

    private:
        size_t indent_count{ 0 };

        std::ostream& write_indent(std::ostream& stream);

        std::ostream& write_begin_block(std::ostream& stream);
        std::ostream& write_end_block(std::ostream& stream);

        std::ostream& write_type(std::ostream& stream, std::type_index type);
        std::ostream& write_construct(std::ostream& stream, std::string_view name, std::type_index type);
        std::ostream& write_call(std::ostream& stream, std::string_view name, std::string_view method, std::string_view args);
        std::ostream& write_set_property(std::ostream& stream, std::string_view name, std::string_view prop, std::string_view value);
        std::ostream& write_add_property(std::ostream& stream, std::string_view name, std::string_view prop, std::string_view value);
        std::ostream& write_add_event(std::ostream& stream, std::string_view name, xaml_event& ev);

        std::ostream& compile_impl(std::ostream& stream, xaml_node& node, bool is_this);
        std::ostream& compile_extensions(std::ostream& stream, xaml_node& node, bool is_this);

    public:
        std::ostream& compile(std::ostream& stream);
    };
} // namespace xaml

#endif // !XAMLC_COMPILER_HPP
