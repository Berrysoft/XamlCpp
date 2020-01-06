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
        std::ostream& write_type(std::ostream& stream, std::type_index type);
        std::ostream& write_construct(std::ostream& stream, xaml_node& node);

    public:
        std::ostream& compile(std::ostream& stream);
    };
} // namespace xaml

#endif // !XAMLC_COMPILER_HPP
