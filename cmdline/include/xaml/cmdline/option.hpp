#ifndef XAML_CMDLINE_OPTION_HPP
#define XAML_CMDLINE_OPTION_HPP

#include <map>
#include <optional>
#include <string>
#include <xaml/meta/meta_macro.hpp>
#include <xaml/strings.hpp>

namespace xaml
{
    namespace cmdline
    {
        class option;
    }

    template <>
    struct type_guid<cmdline::option>
    {
        static constexpr guid value{ 0x240bec4e, 0xefc2, 0x45ed, { 0xa5, 0x01, 0x04, 0xc6, 0x44, 0xf3, 0xf7, 0x0d } };
    };
} // namespace xaml

namespace xaml::cmdline
{
    struct option_entry
    {
        char_t short_arg;
        string_t long_arg;
        string_t help_text;
    };

    class option : public meta_class
    {
    public:
        META_CLASS_IMPL(meta_class)

    private:
        std::map<char_t, size_t> m_short_args{};
        std::map<string_t, size_t> m_long_args{};
        std::map<size_t, std::string> m_properties{};
        std::multimap<std::string, option_entry> m_entries{};
        std::string m_default_property{};

    public:
        option() : meta_class() {}
        ~option() override {}

        XAML_CMDLINE_API std::optional<std::string_view> find_short_arg(char_t name) const noexcept;
        XAML_CMDLINE_API std::optional<std::string_view> find_long_arg(string_view_t name) const noexcept;
        std::string_view get_default_property() const noexcept { return m_default_property; }

        XAML_CMDLINE_API std::shared_ptr<option> add_arg(char_t short_name, string_view_t long_name, std::string_view prop_name, string_view_t help_text = {});

        XAML_CMDLINE_API std::basic_ostream<char_t>& print_help(std::basic_ostream<char_t>& stream) const;

        REGISTER_CLASS_DECL(xaml::cmdline, option, "xaml/cmdline")
        {
            ADD_CTOR();
        }
        REGISTER_CLASS_END()
    };
} // namespace xaml::cmdline

#endif // !XAML_CMDLINE_OPTION_HPP
