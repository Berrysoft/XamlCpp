#ifndef XAML_CONV_HPP
#define XAML_CONV_HPP

#include <any>
#include <locale>
#include <string>
#include <typeinfo>

namespace xaml
{
    struct value_converter
    {
        virtual std::any convert(std::any value, std::type_info const& target_type, std::any param, std::locale language) = 0;
        virtual std::any convert_back(std::any value, std::type_info const& target_type, std::any param, std::locale language) = 0;
    };

    namespace primitives
    {
#define STRING_PRIMITIVE_CONV(name, t, func)                                                         \
    struct name : value_converter                                                                    \
    {                                                                                                \
        std::any convert(std::any value, std::type_info const&, std::any, std::locale) override      \
        {                                                                                            \
            if (value.type() == typeid(std::string))                                                 \
            {                                                                                        \
                return (t)func(std::any_cast<std::string const&>(value));                            \
            }                                                                                        \
            return t{};                                                                              \
        }                                                                                            \
        std::any convert_back(std::any value, std::type_info const&, std::any, std::locale) override \
        {                                                                                            \
            if (value.type() == typeid(t))                                                           \
            {                                                                                        \
                return std::to_string(std::any_cast<t>(value));                                      \
            }                                                                                        \
            return std::string{};                                                                    \
        }                                                                                            \
    };

        STRING_PRIMITIVE_CONV(string_int8_converter, std::int8_t, std::stoi)
        STRING_PRIMITIVE_CONV(string_int16_converter, std::int16_t, std::stoi)
        STRING_PRIMITIVE_CONV(string_int32_converter, std::int32_t, std::stoi)
        STRING_PRIMITIVE_CONV(string_int64_converter, std::int64_t, std::stoll)

        STRING_PRIMITIVE_CONV(string_uint8_converter, std::uint8_t, std::stoul)
        STRING_PRIMITIVE_CONV(string_uint16_converter, std::uint16_t, std::stoul)
        STRING_PRIMITIVE_CONV(string_uint32_converter, std::uint32_t, std::stoul)
        STRING_PRIMITIVE_CONV(string_uint64_converter, std::uint64_t, std::stoull)

        STRING_PRIMITIVE_CONV(string_float_converter, float, std::stof)
        STRING_PRIMITIVE_CONV(string_double_converter, double, std::stod)

#undef STRING_PRIMITIVE_CONV
    } // namespace primitives
} // namespace xaml

#endif // !XAML_CONV_HPP
