#include <sstream>
#include <xaml/cmdline/option.hpp>
#include <xaml/cmdline/parser.hpp>

using namespace std;

namespace xaml::cmdline
{
    no_registered_option::no_registered_option() : logic_error("There's no xaml::cmdline::option attribute registered in the specified type.") {}

    static string get_invalid_option_message(string_view_t opt)
    {
        ostringstream stream;
        stream << "Invalid option: " << quoted(value_converter_traits<string_view>::convert(opt));
        return stream.str();
    }

    invalid_option::invalid_option(string_view_t opt) : logic_error(get_invalid_option_message(opt)) {}

    vector<option_node> parse(reflection_info const* refl, array_view<string_t> args)
    {
        option const* popt = refl->get_attribute<option>();
        if (!popt) throw no_registered_option{};
        vector<option_node> options;
        for (size_t i = 1; i < args.size(); i++)
        {
            string_view_t arg = args[i];
            if (arg.empty())
                continue;
            else if (arg[0] == '-')
            {
                if (arg.size() == 1)
                    throw invalid_option{ arg };
                else if (arg[1] == '-')
                {
                    if (arg.size() == 2) throw invalid_option{ arg };
                    string_view_t long_arg = arg.substr(2);
                    if (auto pprop = popt->find_long_arg(long_arg))
                    {
                        auto prop = refl->get_property(*pprop);
                        if (prop)
                        {
                            i++;
                            options.push_back({ prop, args[i] });
                        }
                    }
                    else
                    {
                        throw invalid_option{ arg };
                    }
                }
                else
                {
                    char_t short_arg = arg[1];
                    string_view_t switches_or_value = arg.size() > 2 ? arg.substr(2) : string_view_t{};
                    if (auto pprop = popt->find_short_arg(short_arg))
                    {
                        auto prop = refl->get_property(*pprop);
                        if (prop)
                        {
                            if (prop->type() == type_index(typeid(bool)))
                            {
                                options.push_back({ prop, U("true") });
                                for (char_t other_short_arg : switches_or_value)
                                {
                                    if (auto pprop = popt->find_short_arg(short_arg))
                                    {
                                        auto prop = refl->get_property(*pprop);
                                        if (prop)
                                        {
                                            options.push_back({ prop, U("true") });
                                        }
                                    }
                                    else
                                    {
                                        throw invalid_option{ arg };
                                    }
                                }
                            }
                            else
                            {
                                if (switches_or_value.empty())
                                {
                                    i++;
                                    options.push_back({ prop, args[i] });
                                }
                                else
                                {
                                    options.push_back({ prop, (string_t)switches_or_value });
                                }
                            }
                        }
                    }
                    else
                    {
                        throw invalid_option{ arg };
                    }
                }
            }
            else
            {
                if (auto pdef_prop = popt->get_default_property())
                {
                    auto prop = refl->get_property(*pdef_prop);
                    if (prop)
                    {
                        options.push_back({ prop, (string_t)arg });
                    }
                }
                else
                {
                    throw invalid_option{ arg };
                }
            }
        }
        return options;
    }
} // namespace xaml::cmdline
