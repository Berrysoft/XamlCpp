#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index_container.hpp>
#include <sf/sformat.hpp>
#include <xaml/cmdline/option.h>
#include <xaml/internal/stream.hpp>
#include <xaml/map.h>

using namespace std;
using namespace boost::multi_index;

struct option_entry
{
    char short_arg;
    string_view long_arg_view;
    xaml_ptr<xaml_string> long_arg;
    xaml_ptr<xaml_string> prop;
    xaml_ptr<xaml_string> help_text;
};

struct xaml_cmdline_option_for_each_args_impl : xaml_implement<xaml_cmdline_option_for_each_args_impl, xaml_cmdline_option_for_each_args>
{
    option_entry m_entry{};

    xaml_cmdline_option_for_each_args_impl(char short_arg, xaml_string* long_arg, xaml_string* prop, xaml_string* help_text) noexcept
        : m_entry{ short_arg, {}, long_arg, prop, help_text } {}

    xaml_result XAML_CALL get_short_arg(char* pvalue) noexcept override
    {
        *pvalue = m_entry.short_arg;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_long_arg(xaml_string** ptr) noexcept override
    {
        return m_entry.long_arg.query(ptr);
    }

    xaml_result XAML_CALL get_prop(xaml_string** ptr) noexcept override
    {
        return m_entry.prop.query(ptr);
    }

    xaml_result XAML_CALL get_help_text(xaml_string** ptr) noexcept override
    {
        return m_entry.help_text.query(ptr);
    }
};

xaml_result XAML_CALL xaml_cmdline_option_for_each_args_new(char short_arg, xaml_string* long_arg, xaml_string* prop, xaml_string* help_text, xaml_cmdline_option_for_each_args** ptr) noexcept
{
    return xaml_object_new<xaml_cmdline_option_for_each_args_impl>(ptr, short_arg, long_arg, prop, help_text);
}

struct xaml_cmdline_option_impl : xaml_implement<xaml_cmdline_option_impl, xaml_cmdline_option>
{
    multi_index_container<
        option_entry,
        indexed_by<
            hashed_non_unique<
                member<
                    option_entry,
                    char,
                    &option_entry::short_arg>>,
            hashed_non_unique<
                member<
                    option_entry,
                    string_view,
                    &option_entry::long_arg_view>>>>
        m_map;

    xaml_result XAML_CALL find_short_arg(char name, xaml_string** ptr) noexcept override;
    xaml_result XAML_CALL find_long_arg(xaml_string* name, xaml_string** ptr) noexcept override;
    xaml_result XAML_CALL get_default_property(xaml_string** ptr) noexcept override;
    xaml_result XAML_CALL add_arg(char, xaml_string*, xaml_string*, xaml_string*) noexcept override;
    xaml_result XAML_CALL for_each_entry(xaml_delegate<xaml_object, xaml_cmdline_option_for_each_args>* handler) noexcept override;
};

xaml_result xaml_cmdline_option_impl::find_short_arg(char name, xaml_string** ptr) noexcept
try
{
    auto& short_index = m_map.get<0>();
    auto [begin, end] = short_index.equal_range(name);
    if (begin == end) return XAML_E_KEYNOTFOUND;
    return begin->prop->query(ptr);
}
XAML_CATCH_RETURN()

xaml_result xaml_cmdline_option_impl::find_long_arg(xaml_string* name, xaml_string** ptr) noexcept
try
{
    auto& long_index = m_map.get<1>();
    string_view view;
    XAML_RETURN_IF_FAILED(to_string_view(name, &view));
    auto [begin, end] = long_index.equal_range(view);
    if (begin == end) return XAML_E_KEYNOTFOUND;
    return begin->prop->query(ptr);
}
XAML_CATCH_RETURN()

xaml_result xaml_cmdline_option_impl::get_default_property(xaml_string** ptr) noexcept
try
{
    auto& short_index = m_map.get<0>();
    auto [begin, end] = short_index.equal_range('\0');
    if (begin == end) return XAML_E_KEYNOTFOUND;
    for (auto it = begin; it != end; ++it)
    {
        if (it->long_arg_view.empty()) return it->prop->query(ptr);
    }
    return XAML_E_KEYNOTFOUND;
}
XAML_CATCH_RETURN()

xaml_result xaml_cmdline_option_impl::add_arg(char short_name, xaml_string* long_name, xaml_string* prop_name, xaml_string* help_text) noexcept
try
{
    xaml_ptr<xaml_string> rln = long_name;
    if (!rln) XAML_RETURN_IF_FAILED(xaml_string_empty(&rln));
    std::string_view rln_view;
    XAML_RETURN_IF_FAILED(to_string_view(long_name, &rln_view));
    xaml_ptr<xaml_string> rpn = prop_name;
    if (!rpn) XAML_RETURN_IF_FAILED(xaml_string_empty(&rpn));
    xaml_ptr<xaml_string> rht = help_text;
    if (!rht) XAML_RETURN_IF_FAILED(xaml_string_empty(&rht));
    m_map.insert({ short_name, rln_view, rln, rpn, rht });
    return XAML_S_OK;
}
XAML_CATCH_RETURN()

xaml_result xaml_cmdline_option_impl::for_each_entry(xaml_delegate<xaml_object, xaml_cmdline_option_for_each_args>* handler) noexcept
{
    for (auto& entry : m_map)
    {
        xaml_ptr<xaml_cmdline_option_for_each_args> args;
        XAML_RETURN_IF_FAILED(xaml_cmdline_option_for_each_args_new(entry.short_arg, entry.long_arg, entry.prop, entry.help_text, &args));
        XAML_RETURN_IF_FAILED(handler->invoke(this, args));
    }
    return XAML_S_OK;
}

xaml_result XAML_CALL xaml_cmdline_option_new(xaml_cmdline_option** ptr) noexcept
{
    return xaml_object_new_catch<xaml_cmdline_option_impl>(ptr);
}

static xaml_result XAML_CALL xaml_cmdline_option_print_impl(basic_ostream<char>& stream, xaml_cmdline_option* opt) noexcept
{
    xaml_ptr<xaml_delegate<xaml_object, xaml_cmdline_option_for_each_args>> callback;
    XAML_RETURN_IF_FAILED((xaml_delegate_new(
        [&stream](xaml_object*, xaml_cmdline_option_for_each_args* e) noexcept -> xaml_result {
            try
            {
                char short_arg;
                XAML_RETURN_IF_FAILED(e->get_short_arg(&short_arg));
                xaml_ptr<xaml_string> long_arg;
                XAML_RETURN_IF_FAILED(e->get_long_arg(&long_arg));
                std::string arg_str;
                std::string_view long_arg_view;
                XAML_RETURN_IF_FAILED(to_string_view(long_arg, &long_arg_view));
                if (short_arg)
                {
                    if (!long_arg_view.empty())
                    {
                        arg_str = sf::sprint(U("-{}, --{}"), short_arg, long_arg_view);
                    }
                    else
                    {
                        arg_str = sf::sprint(U("-{}"), short_arg);
                    }
                }
                else
                {
                    if (!long_arg_view.empty())
                    {
                        arg_str = sf::sprint(U("    --{}"), long_arg_view);
                    }
                    else
                    {
                        arg_str = U("[default]");
                    }
                }
                sf::print(stream, U("  {:l22}"), arg_str);
                xaml_ptr<xaml_string> help_text;
                XAML_RETURN_IF_FAILED(e->get_help_text(&help_text));
                string_view help_text_data;
                XAML_RETURN_IF_FAILED(to_string_view(help_text, &help_text_data));
                sf::println(stream, help_text_data);
                return XAML_S_OK;
            }
            XAML_CATCH_RETURN()
        },
        &callback)));
    return opt->for_each_entry(callback);
}

xaml_result XAML_CALL xaml_cmdline_option_print(FILE* file, xaml_cmdline_option* opt) noexcept
{
    return cfile_ostream_invoke<char>(xaml_cmdline_option_print_impl, file, opt);
}

xaml_result XAML_CALL xaml_cmdline_option_print(basic_ostream<char>& stream, xaml_cmdline_option* opt) noexcept
{
    return xaml_cmdline_option_print_impl(stream, opt);
}
