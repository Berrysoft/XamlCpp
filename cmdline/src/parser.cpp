#include <sstream>
#include <xaml/cmdline/option.hpp>
#include <xaml/cmdline/parser.hpp>

using namespace std;

namespace xaml::cmdline
{
    void init_parser(meta_context& ctx) noexcept
    {
        register_class<option>(ctx);
    }

    no_registered_option::no_registered_option() : logic_error("There's no xaml::cmdline::option attribute registered in the specified type.") {}

    static string get_invalid_option_message(string_view_t opt)
    {
        ostringstream stream;
        stream << "Invalid option: " << quoted(to_string(opt));
        return stream.str();
    }

    invalid_option::invalid_option(string_view_t opt) : logic_error(get_invalid_option_message(opt)) {}

    options parse(reflection_info const* refl, array_view<string_t> args)
    {
        auto popt = refl->get_attribute<option>();
        if (!popt) throw no_registered_option{};
        options result;
        for (size_t i = 0; i < args.size(); i++)
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
                            if (prop->type() == type_guid_v<meta_box<bool>>)
                            {
                                result.properties.push_back({ prop, U("true") });
                            }
                            else
                            {
                                i++;
                                result.properties.push_back({ prop, args[i] });
                            }
                        }
                        else
                        {
                            auto cprop = refl->get_collection_property(*pprop);
                            if (cprop)
                            {
                                result.collection_properties[(string)*pprop].info = cprop;
                                i++;
                                result.collection_properties[(string)*pprop].values.push_back(args[i]);
                            }
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
                            if (prop->type() == type_guid_v<meta_box<bool>>)
                            {
                                result.properties.push_back({ prop, U("true") });
                                for (char_t other_short_arg : switches_or_value)
                                {
                                    if (auto pprop = popt->find_short_arg(other_short_arg))
                                    {
                                        auto prop = refl->get_property(*pprop);
                                        if (prop)
                                        {
                                            result.properties.push_back({ prop, U("true") });
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
                                    result.properties.push_back({ prop, args[i] });
                                }
                                else
                                {
                                    result.properties.push_back({ prop, (string_t)switches_or_value });
                                }
                            }
                        }
                        else
                        {
                            auto cprop = refl->get_collection_property(*pprop);
                            if (cprop)
                            {
                                result.collection_properties[(string)*pprop].info = cprop;
                                if (switches_or_value.empty())
                                {
                                    i++;
                                    result.collection_properties[(string)*pprop].values.push_back(args[i]);
                                }
                                else
                                {
                                    result.collection_properties[(string)*pprop].values.emplace_back(switches_or_value);
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
                auto pdef_prop = popt->get_default_property();
                if (!pdef_prop.empty())
                {
                    auto prop = refl->get_property(pdef_prop);
                    if (prop)
                    {
                        result.properties.push_back({ prop, (string_t)arg });
                    }
                    else
                    {
                        auto cprop = refl->get_collection_property(pdef_prop);
                        if (cprop)
                        {
                            result.collection_properties[(string)pdef_prop].info = cprop;
                            result.collection_properties[(string)pdef_prop].values.emplace_back(arg);
                        }
                    }
                }
                else
                {
                    throw invalid_option{ arg };
                }
            }
        }
        return result;
    }
} // namespace xaml::cmdline
