#include <iomanip>
#include <nowide/args.hpp>
#include <nowide/iostream.hpp>
#include <options.h>
#include <sf/format.hpp>
#include <unordered_map>
#include <xaml/version.h>

using nowide::cout;

std::string_view get_type_name(xaml_ptr<xaml_meta_context> const& ctx, xaml_guid const& type)
{
    xaml_ptr<xaml_reflection_info> info;
    if (XAML_SUCCEEDED(ctx->get_type(type, &info)))
    {
        xaml_ptr<xaml_string> name;
        XAML_THROW_IF_FAILED(info->get_name(&name));
        return to_string_view(name);
    }
    else
    {
        return U("Unknown-type");
    }
}

int main(int argc, char** argv)
{
    nowide::args _(argc, argv);

    xaml_ptr<xaml_meta_context> cmdline_ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&cmdline_ctx));
    XAML_THROW_IF_FAILED(xaml_detector_options_register(cmdline_ctx));
    xaml_ptr<xaml_detector_options> options;
    XAML_THROW_IF_FAILED(xaml_cmdline_parse_and_print(cmdline_ctx, argc, argv, &options));

    xaml_ptr<xaml_string> path_str;
    XAML_THROW_IF_FAILED(options->get_path(&path_str));

    xaml_ptr<xaml_module> m;
    XAML_THROW_IF_FAILED(xaml_module_new(&m));
    XAML_THROW_IF_FAILED(m->open(path_str));

    xaml_ptr<xaml_meta_context> ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&ctx));

    bool recursive;
    XAML_THROW_IF_FAILED(options->get_recursive(&recursive));
    if (recursive)
        XAML_THROW_IF_FAILED(ctx->add_module_recursive(m));
    else
        XAML_THROW_IF_FAILED(ctx->add_module(m));

    xaml_version ver;
    {
        xaml_ptr<xaml_module_info> info;
        XAML_RETURN_IF_FAILED(m->get_info(&info));
        XAML_THROW_IF_FAILED(info->get_version(&ver));
    }

    sf::println(cout, U("Module {} ({})"), quoted(to_string_view(path_str)), ver);

    xaml_ptr<xaml_map_view<xaml_guid, xaml_reflection_info>> types;
    XAML_THROW_IF_FAILED(ctx->get_types(&types));

    for (auto pair : xaml_enumerable_wrapper<xaml_key_value_pair<xaml_guid, xaml_reflection_info>>{ types })
    {
        xaml_ptr<xaml_reflection_info> info;
        XAML_THROW_IF_FAILED(pair->get_value(&info));
        xaml_ptr<xaml_string> name;
        XAML_THROW_IF_FAILED(info->get_name(&name));
        xaml_ptr<xaml_string> include_file;
        XAML_THROW_IF_FAILED(info->get_include_file(&include_file));
        if (auto t = info.query<xaml_type_info>())
        {
            sf::println(cout, U("class {}"), name);
            sf::println(cout, U("  included in <{}>"), include_file);
            {
                xaml_ptr<xaml_map_view<xaml_string, xaml_property_info>> props;
                XAML_THROW_IF_FAILED(t->get_properties(&props));
                for (auto pair2 : xaml_enumerable_wrapper<xaml_key_value_pair<xaml_string, xaml_property_info>>{ props })
                {
                    xaml_ptr<xaml_string> key;
                    XAML_THROW_IF_FAILED(pair2->get_key(&key));
                    xaml_ptr<xaml_property_info> info2;
                    XAML_THROW_IF_FAILED(pair2->get_value(&info2));
                    xaml_guid type;
                    XAML_THROW_IF_FAILED(info2->get_type(&type));
                    sf::println(cout, U("    P: {}\t{}"), key, get_type_name(ctx, type));
                }
            }
            {
                xaml_ptr<xaml_map_view<xaml_string, xaml_collection_property_info>> props;
                XAML_THROW_IF_FAILED(t->get_collection_properties(&props));
                for (auto pair2 : xaml_enumerable_wrapper<xaml_key_value_pair<xaml_string, xaml_collection_property_info>>{ props })
                {
                    xaml_ptr<xaml_string> key;
                    XAML_THROW_IF_FAILED(pair2->get_key(&key));
                    xaml_ptr<xaml_collection_property_info> info2;
                    XAML_THROW_IF_FAILED(pair2->get_value(&info2));
                    xaml_guid type;
                    XAML_THROW_IF_FAILED(info2->get_type(&type));
                    sf::println(cout, U("    C: {}\t{}"), key, get_type_name(ctx, type));
                }
            }
            {
                xaml_ptr<xaml_map_view<xaml_string, xaml_event_info>> props;
                XAML_THROW_IF_FAILED(t->get_events(&props));
                for (auto pair2 : xaml_enumerable_wrapper<xaml_key_value_pair<xaml_string, xaml_event_info>>{ props })
                {
                    xaml_ptr<xaml_string> key;
                    XAML_THROW_IF_FAILED(pair2->get_key(&key));
                    sf::println(cout, U("    E: {}"), key);
                }
            }
        }
        else if (auto e = info.query<xaml_enum_info>())
        {
            sf::println(cout, U("enum {}"), name);
            sf::println(cout, U("  included in <{}>"), include_file);
            xaml_ptr<xaml_map_view<xaml_string, int32_t>> values;
            XAML_THROW_IF_FAILED(e->get_values(&values));
            for (auto pair2 : xaml_enumerable_wrapper<xaml_key_value_pair<xaml_string, int32_t>>{ values })
            {
                xaml_ptr<xaml_string> key;
                XAML_THROW_IF_FAILED(pair2->get_key(&key));
                int32_t value;
                XAML_THROW_IF_FAILED(pair2->get_value(&value));
                sf::println(cout, U("    {} = {}"), key.query<xaml_string>(), value);
            }
        }
    }

    return 0;
}
