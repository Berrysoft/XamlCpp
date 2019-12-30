#ifndef XAML_DESERIALIZE_HPP
#define XAML_DESERIALIZE_HPP

#include <libxml/xmlreader.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <xaml/meta/meta.hpp>

namespace xaml
{
    struct xaml_no_default_constructor : std::logic_error
    {
        xaml_no_default_constructor(std::type_index t);
        ~xaml_no_default_constructor() override {}
    };

    struct xaml_bad_type : std::logic_error
    {
        xaml_bad_type(std::string_view ns, std::string_view name);
        ~xaml_bad_type() override {}
    };

    struct xaml_parse_error : std::logic_error
    {
        xaml_parse_error() : logic_error("Failed to parse XAML file.") {}
        ~xaml_parse_error() override {}
    };

    class deserializer
    {
    private:
        xmlTextReaderPtr reader;

    public:
        constexpr bool is_open() const noexcept { return reader; }
        void open(std::string_view file);
        constexpr operator bool() const noexcept { return reader; }

        deserializer();
        deserializer(std::string_view file) : deserializer() { open(file); }
        ~deserializer();

    private:
        int deserialize_members(std::shared_ptr<meta_class> mc, std::shared_ptr<meta_class> root);
        std::tuple<int, std::shared_ptr<meta_class>> deserialize_impl(std::shared_ptr<meta_class> root);
        void clean_up(int ret);

    public:
        void deserialize(std::shared_ptr<meta_class> mc);
    };
} // namespace xaml

#endif // !XAML_DESERIALIZE_HPP
