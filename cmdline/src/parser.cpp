#include <sstream>
#include <xaml/cmdline/option.h>
#include <xaml/cmdline/parser.h>

using namespace std;

struct xaml_cmdline_options_impl : xaml_implement<xaml_cmdline_options_impl, xaml_cmdline_options, xaml_object>
{
    XAML_PROP_PTR_IMPL(properties, xaml_map)
    XAML_PROP_PTR_IMPL(collection_properties, xaml_map)
};

template <typename S2>
static xaml_result props_insert(xaml_ptr<xaml_map> const& props, xaml_ptr<xaml_property_info> const& info, S2&& value) noexcept
{
    xaml_ptr<xaml_object> value_str;
    XAML_RETURN_IF_FAILED(xaml_box_value(std::forward<S2>(value), &value_str));
    bool replaced;
    return props->insert(info, value_str, &replaced);
}

template <typename S>
static xaml_result cprops_values(xaml_ptr<xaml_map> const& cprops, S&& str, xaml_ptr<xaml_collection_property_info> const& cprop, xaml_vector** ptr) noexcept
{
    xaml_ptr<xaml_object> key_str;
    XAML_RETURN_IF_FAILED(xaml_box_value(std::forward<S>(str), &key_str));
    xaml_ptr<xaml_vector> values;
    {
        xaml_ptr<xaml_key_value_pair> pair;
        xaml_ptr<xaml_object> item;
        if (XAML_SUCCEEDED(cprops->lookup(key_str, &item)))
        {
            XAML_RETURN_IF_FAILED(item->query(&pair));
            xaml_ptr<xaml_object> values_item;
            XAML_RETURN_IF_FAILED(pair->get_value(&values_item));
            XAML_RETURN_IF_FAILED(values_item->query(&values));
        }
        else
        {
            XAML_RETURN_IF_FAILED(xaml_vector_new(&values));
            XAML_RETURN_IF_FAILED(xaml_key_value_pair_new(cprop, values, &pair));
            bool replaced;
            XAML_RETURN_IF_FAILED(cprops->insert(key_str, pair, &replaced));
        }
    }
    return values->query(ptr);
}

xaml_result XAML_CALL xaml_cmdline_parse(xaml_type_info* type, xaml_vector_view* args, xaml_cmdline_options** ptr) noexcept
{
    xaml_ptr<xaml_cmdline_option> popt;
    XAML_RETURN_IF_FAILED(type->get_attribute(&popt));
    xaml_ptr<xaml_map> props;
    XAML_RETURN_IF_FAILED(xaml_map_new(&props));
    xaml_ptr<xaml_map> cprops;
    XAML_RETURN_IF_FAILED(xaml_map_new(&cprops));
    int32_t size;
    XAML_RETURN_IF_FAILED(args->get_size(&size));
    for (int32_t i = 0; i < size; i++)
    {
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(args->get_at(i, &item));
        xaml_ptr<xaml_string> arg_item;
        XAML_RETURN_IF_FAILED(item->query(&arg_item));
        xaml_std_string_view_t arg;
        XAML_RETURN_IF_FAILED(to_string_view_t(arg_item, &arg));
        if (arg.empty())
            continue;
        else if (arg[0] == '-')
        {
            if (arg.size() == 1)
                return XAML_E_FAIL;
            else if (arg[1] == '-')
            {
                if (arg.size() == 2) return XAML_E_FAIL;
                xaml_std_string_view_t long_arg = arg.substr(2);
                size_t index = long_arg.find_first_of('=');
                xaml_std_string_view_t maybe_value = {};
                if (index != xaml_std_string_view_t::npos)
                {
                    maybe_value = long_arg.substr(index + 1);
                    long_arg = long_arg.substr(0, index);
                }
                xaml_ptr<xaml_string> long_arg_str;
                XAML_RETURN_IF_FAILED(xaml_string_new_view(long_arg, &long_arg_str));
                xaml_ptr<xaml_string> pprop;
                XAML_RETURN_IF_FAILED(popt->find_long_arg(long_arg_str, &pprop));
                xaml_ptr<xaml_property_info> prop;
                if (XAML_SUCCEEDED(type->get_property(pprop, &prop)))
                {
                    if (!maybe_value.empty())
                    {
                        XAML_RETURN_IF_FAILED(props_insert(props, prop, maybe_value));
                    }
                    else
                    {
                        xaml_guid t;
                        XAML_RETURN_IF_FAILED(prop->get_type(&t));
                        if (t == xaml_type_guid_v<bool>)
                        {
                            XAML_RETURN_IF_FAILED(props_insert(props, prop, U("true")));
                        }
                        else
                        {
                            i++;
                            xaml_ptr<xaml_object> new_item;
                            XAML_RETURN_IF_FAILED(args->get_at(i, &new_item));
                            XAML_RETURN_IF_FAILED(props_insert(props, prop, new_item));
                        }
                    }
                }
                else
                {
                    xaml_ptr<xaml_collection_property_info> cprop;
                    if (XAML_SUCCEEDED(type->get_collection_property(pprop, &cprop)))
                    {
                        xaml_ptr<xaml_vector> values;
                        XAML_RETURN_IF_FAILED(cprops_values(cprops, pprop, cprop, &values));
                        if (!maybe_value.empty())
                        {
                            xaml_ptr<xaml_string> maybe_value_str;
                            XAML_RETURN_IF_FAILED(xaml_string_new(maybe_value, &maybe_value_str));
                            XAML_RETURN_IF_FAILED(values->append(maybe_value_str));
                        }
                        else
                        {
                            i++;
                            xaml_ptr<xaml_object> new_item;
                            XAML_RETURN_IF_FAILED(args->get_at(i, &new_item));
                            xaml_ptr<xaml_string> new_arg;
                            XAML_RETURN_IF_FAILED(new_item->query(&new_arg));
                            XAML_RETURN_IF_FAILED(values->append(new_arg));
                        }
                    }
                }
            }
            else
            {
                xaml_char_t short_arg = arg[1];
                xaml_std_string_view_t switches_or_value = arg.size() > 2 ? arg.substr(2) : xaml_std_string_view_t{};
                xaml_ptr<xaml_string> pprop;
                XAML_RETURN_IF_FAILED(popt->find_short_arg(short_arg, &pprop));
                xaml_ptr<xaml_property_info> prop;
                if (XAML_SUCCEEDED(type->get_property(pprop, &prop)))
                {
                    xaml_guid t;
                    XAML_RETURN_IF_FAILED(prop->get_type(&t));
                    if (t == xaml_type_guid_v<bool>)
                    {
                        XAML_RETURN_IF_FAILED(props_insert(props, prop, U("true")));
                        for (xaml_char_t other_short_arg : switches_or_value)
                        {
                            xaml_ptr<xaml_string> pprop;
                            XAML_RETURN_IF_FAILED(popt->find_short_arg(other_short_arg, &pprop));
                            xaml_ptr<xaml_property_info> prop;
                            if (XAML_SUCCEEDED(type->get_property(pprop, &prop)))
                            {
                                XAML_RETURN_IF_FAILED(props_insert(props, prop, U("true")));
                            }
                        }
                    }
                    else
                    {
                        if (switches_or_value.empty())
                        {
                            i++;
                            xaml_ptr<xaml_object> new_item;
                            XAML_RETURN_IF_FAILED(args->get_at(i, &new_item));
                            XAML_RETURN_IF_FAILED(props_insert(props, prop, new_item));
                        }
                        else
                        {
                            XAML_RETURN_IF_FAILED(props_insert(props, prop, switches_or_value));
                        }
                    }
                }
                else
                {
                    xaml_ptr<xaml_collection_property_info> cprop;
                    if (XAML_SUCCEEDED(type->get_collection_property(pprop, &cprop)))
                    {
                        xaml_ptr<xaml_vector> values;
                        XAML_RETURN_IF_FAILED(cprops_values(cprops, pprop, cprop, &values));
                        if (!switches_or_value.empty())
                        {
                            xaml_ptr<xaml_string> value_str;
                            XAML_RETURN_IF_FAILED(xaml_string_new(switches_or_value, &value_str));
                            XAML_RETURN_IF_FAILED(values->append(value_str));
                        }
                        else
                        {
                            i++;
                            xaml_ptr<xaml_object> new_item;
                            XAML_RETURN_IF_FAILED(args->get_at(i, &new_item));
                            xaml_ptr<xaml_string> new_arg;
                            XAML_RETURN_IF_FAILED(new_item->query(&new_arg));
                            XAML_RETURN_IF_FAILED(values->append(new_arg));
                        }
                    }
                }
            }
        }
        else
        {
            xaml_ptr<xaml_string> pdef_prop;
            XAML_RETURN_IF_FAILED(popt->get_default_property(&pdef_prop));
            xaml_ptr<xaml_property_info> prop;
            if (XAML_SUCCEEDED(type->get_property(pdef_prop, &prop)))
            {
                XAML_RETURN_IF_FAILED(props_insert(props, prop, arg_item));
            }
            else
            {
                xaml_ptr<xaml_collection_property_info> cprop;
                if (XAML_SUCCEEDED(type->get_collection_property(pdef_prop, &cprop)))
                {
                    xaml_ptr<xaml_vector> values;
                    XAML_RETURN_IF_FAILED(cprops_values(cprops, pdef_prop, cprop, &values));
                    XAML_RETURN_IF_FAILED(values->append(arg_item));
                }
            }
        }
    }
    xaml_ptr<xaml_cmdline_options> result;
    XAML_RETURN_IF_FAILED(xaml_object_new<xaml_cmdline_options_impl>(&result));
    XAML_RETURN_IF_FAILED(result->set_properties(props));
    XAML_RETURN_IF_FAILED(result->set_collection_properties(cprops));
    return result->query(ptr);
}

xaml_result XAML_CALL xaml_cmdline_parse_argv(xaml_type_info* type, int argc, xaml_char_t** argv, xaml_cmdline_options** ptr) noexcept
{
    xaml_ptr<xaml_vector> args;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&args));
    for (int i = 1; i < argc; i++)
    {
        xaml_ptr<xaml_string> arg;
        XAML_RETURN_IF_FAILED(xaml_string_new_view(argv[i], &arg));
        XAML_RETURN_IF_FAILED(args->append(arg));
    }
    return xaml_cmdline_parse(type, args, ptr);
}
