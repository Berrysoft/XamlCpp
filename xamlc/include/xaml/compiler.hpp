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

    public:
        std::ostream& compile(std::ostream& stream);
    };
} // namespace xaml

#endif // !XAMLC_COMPILER_HPP
