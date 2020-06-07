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

    xaml_ptr<xaml_module> module;
    XAML_THROW_IF_FAILED(xaml_module_new(&module));
    XAML_THROW_IF_FAILED(module->open(path_str));

    xaml_ptr<xaml_meta_context> ctx;
    XAML_THROW_IF_FAILED(xaml_meta_context_new(&ctx));

    bool recursive;
    XAML_THROW_IF_FAILED(options->get_recursive(&recursive));
    if (recursive)
        XAML_THROW_IF_FAILED(ctx->add_module_recursive(module));
    else
        XAML_THROW_IF_FAILED(ctx->add_module(module));

    xaml_result (*pget_version)(xaml_version*);
    XAML_THROW_IF_FAILED(module->get_method("xaml_module_version", &pget_version));

    xaml_version ver;
    XAML_THROW_IF_FAILED(pget_version(&ver));

    sf::println(cout, U("Module {} ({})"), quoted(to_string_view(path_str)), ver);

    xaml_ptr<xaml_map_view> types;
    XAML_THROW_IF_FAILED(ctx->get_types(&types));

    for (auto item : types)
    {
        xaml_ptr<xaml_key_value_pair> pair = item.query<xaml_key_value_pair>();
        xaml_ptr<xaml_object> second;
        XAML_THROW_IF_FAILED(pair->get_value(&second));
        xaml_ptr<xaml_reflection_info> info = second.query<xaml_reflection_info>();
        xaml_ptr<xaml_string> name;
        XAML_THROW_IF_FAILED(info->get_name(&name));
        xaml_ptr<xaml_string> include_file;
        XAML_THROW_IF_FAILED(info->get_include_file(&include_file));
        if (auto t = info.query<xaml_type_info>())
        {
            sf::println(cout, U("class {}"), name);
            sf::println(cout, U("  included in <{}>"), include_file);
            {
                xaml_ptr<xaml_map_view> props;
                XAML_THROW_IF_FAILED(t->get_properties(&props));
                for (auto item2 : props)
                {
                    xaml_ptr<xaml_key_value_pair> pair2 = item2.query<xaml_key_value_pair>();
                    xaml_ptr<xaml_object> key;
                    XAML_THROW_IF_FAILED(pair2->get_key(&key));
                    xaml_ptr<xaml_object> value;
                    XAML_THROW_IF_FAILED(pair2->get_value(&value));
                    xaml_ptr<xaml_property_info> info2 = value.query<xaml_property_info>();
                    xaml_guid type;
                    XAML_THROW_IF_FAILED(info2->get_type(&type));
                    sf::println(cout, U("    P: {}\t{}"), key.query<xaml_string>(), get_type_name(ctx, type));
                }
            }
            {
                xaml_ptr<xaml_map_view> props;
                XAML_THROW_IF_FAILED(t->get_collection_properties(&props));
                for (auto item2 : props)
                {
                    xaml_ptr<xaml_key_value_pair> pair2 = item2.query<xaml_key_value_pair>();
                    xaml_ptr<xaml_object> key;
                    XAML_THROW_IF_FAILED(pair2->get_key(&key));
                    xaml_ptr<xaml_object> value;
                    XAML_THROW_IF_FAILED(pair2->get_value(&value));
                    xaml_ptr<xaml_collection_property_info> info2 = value.query<xaml_collection_property_info>();
                    xaml_guid type;
                    XAML_THROW_IF_FAILED(info2->get_type(&type));
                    sf::println(cout, U("    C: {}\t{}"), key.query<xaml_string>(), get_type_name(ctx, type));
                }
            }
            {
                xaml_ptr<xaml_map_view> props;
                XAML_THROW_IF_FAILED(t->get_events(&props));
                for (auto item2 : props)
                {
                    xaml_ptr<xaml_key_value_pair> pair2 = item2.query<xaml_key_value_pair>();
                    xaml_ptr<xaml_object> key;
                    XAML_THROW_IF_FAILED(pair2->get_key(&key));
                    sf::println(cout, U("    E: {}"), key.query<xaml_string>());
                }
            }
        }
        else if (auto e = info.query<xaml_enum_info>())
        {
            sf::println(cout, U("enum {}"), name);
            sf::println(cout, U("  included in <{}>"), include_file);
            xaml_ptr<xaml_map_view> values;
            XAML_THROW_IF_FAILED(e->get_values(&values));
            for (auto item2 : values)
            {
                xaml_ptr<xaml_key_value_pair> pair2 = item2.query<xaml_key_value_pair>();
                xaml_ptr<xaml_object> key;
                XAML_THROW_IF_FAILED(pair2->get_key(&key));
                xaml_ptr<xaml_object> value;
                XAML_THROW_IF_FAILED(pair2->get_value(&value));
                xaml_ptr<xaml_box> box = value.query<xaml_box>();
                int const* pvalue;
                XAML_THROW_IF_FAILED(box->get_data((void const**)&pvalue));
                sf::println(cout, U("    {} = {}"), key.query<xaml_string>(), *pvalue);
            }
        }
    }

    return 0;
}
