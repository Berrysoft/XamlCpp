#include <iostream>
#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_document.hpp>
#include <sstream>
#include <xaml/markup/binding.h>
#include <xaml/parser/parser.h>

using namespace std;
using namespace rapidxml;

xaml_result XAML_CALL xaml_parser_register(xaml_meta_context* ctx) noexcept
{
    XAML_RETURN_IF_FAILED(xaml_binding_register(ctx));
    // binding_mode
    return XAML_S_OK;
}

static xaml_result get_random_name(xaml_ptr<xaml_type_info> const& ref, xaml_string** ptr) noexcept
{
    static size_t index = 0;
    xaml_ptr<xaml_string> name;
    XAML_RETURN_IF_FAILED(ref->get_name(&name));
    try
    {
        basic_ostringstream<xaml_char_t> oss;
        oss << "__" << name << "__" << index++;
        return xaml_string_new(oss.str(), ptr);
    }
    XAML_CATCH_RETURN()
}

struct parser_impl
{
    xaml_ptr<xaml_meta_context> ctx{ nullptr };
    xaml_ptr<xaml_vector> headers{};
    xml_document doc{};

    xaml_result load_string(string_view s) noexcept
    try
    {
        doc.load_string(s);
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

    xaml_result load_stream(istream& stream) noexcept
    try
    {
        doc.load_stream(stream);
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

    xaml_result load_stream(FILE* stream) noexcept
    try
    {
        doc.load_stream(stream);
        return XAML_S_OK;
    }
    XAML_CATCH_RETURN()

    xaml_result init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&headers));
        return XAML_S_OK;
    }

    xaml_result parse_markup(string_view value, xaml_markup_node** ptr) noexcept;
    xaml_result parse_members(xaml_ptr<xaml_node> const& mc, xml_node& node) noexcept;
    xaml_result parse_impl(xml_node& node, xaml_ptr<xaml_type_info> const& t, xaml_node** ptr) noexcept;
    xaml_result parse(xaml_node** ptr) noexcept;
};

xaml_result parser_impl::parse_markup(string_view value, xaml_markup_node** ptr) noexcept
{
    string_view ns, name;
    int32_t sep_index = 0;
    int32_t i = 0;
    for (; i < value.length(); i++)
    {
        if (!ns.empty() && value[i] == ':')
        {
            ns = value.substr(0, i);
            sep_index = i + 1;
        }
        else if (isspace(value[i]))
        {
            name = value.substr(sep_index, i - sep_index);
            break;
        }
    }
    if (ns.empty()) ns = "xaml";
    xaml_ptr<xaml_reflection_info> info;
    {
        xaml_ptr<xaml_string> ns_str;
        XAML_RETURN_IF_FAILED(xaml_string_new_utf8(ns, &ns_str));
        xaml_ptr<xaml_string> name_str;
        XAML_RETURN_IF_FAILED(xaml_string_new_utf8(name, &name_str));
        XAML_RETURN_IF_FAILED(ctx->get_type_by_namespace_name(ns_str.get(), name_str.get(), &info));
    }
    xaml_ptr<xaml_type_info> t;
    XAML_RETURN_IF_FAILED(info->query(&t));
    xaml_ptr<xaml_string> include_file;
    XAML_RETURN_IF_FAILED(t->get_include_file(&include_file));
    if (include_file)
        headers->append(include_file.get());
    xaml_ptr<xaml_string> node_name;
    XAML_RETURN_IF_FAILED(get_random_name(t, &node_name));
    xaml_ptr<xaml_markup_node> node;
    XAML_RETURN_IF_FAILED(xaml_markup_node_new(&node));
    XAML_RETURN_IF_FAILED(node->set_type(t.get()));
    XAML_RETURN_IF_FAILED(node->set_name(node_name.get()));
    xaml_ptr<xaml_vector> props;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&props));
    while (i < value.length())
    {
        while (i < value.length() && isspace(value[i])) i++;
        int32_t start_index = i;
        for (; i < value.length(); i++)
        {
            if (value[i] == ',')
            {
                i = start_index;
                break;
            }
            else if (value[i] == '=')
                break;
        }
        string_view prop_name = value.substr(start_index, i - start_index);
        while (i < value.length() && value[i] == '=') i++;
        start_index = i;
        for (; i < value.length(); i++)
        {
            if (value[i] == ',') break;
        }
        string_view prop_value = value.substr(start_index, i - start_index);
        while (i < value.length() && value[i] == ',') i++;
        xaml_ptr<xaml_string> prop_name_str;
        if (prop_name.empty())
        {
            xaml_ptr<xaml_default_property> def_attr;
            if (XAML_SUCCEEDED(t->get_attribute(&def_attr)))
            {
                XAML_RETURN_IF_FAILED(def_attr->get_default_property(&prop_name_str));
            }
        }
        else
        {
            XAML_RETURN_IF_FAILED(xaml_string_new_utf8(prop_name, &prop_name_str));
        }
        xaml_ptr<xaml_property_info> prop;
        XAML_RETURN_IF_FAILED(t->get_property(prop_name_str.get(), &prop));
        bool can_write;
        XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
        if (can_write)
        {
            xaml_ptr<xaml_string> prop_value_str;
            XAML_RETURN_IF_FAILED(xaml_string_new_utf8(prop_value, &prop_value_str));
            xaml_ptr<xaml_string_node> node;
            XAML_RETURN_IF_FAILED(xaml_string_node_new(&node));
            XAML_RETURN_IF_FAILED(node->set_value(prop_value_str.get()));
            xaml_ptr<xaml_attribute_property> prop_item;
            XAML_RETURN_IF_FAILED(xaml_attribute_property_new(t.get(), prop.get(), node.get(), &prop_item));
            XAML_RETURN_IF_FAILED(props->append(prop_item.get()));
        }
    }
    XAML_RETURN_IF_FAILED(node->set_properties(props.get()));
    return node->query(ptr);
}

static constexpr string_view x_ns{ "https://github.com/Berrysoft/XamlCpp/xaml/" };

xaml_result parser_impl::parse_members(xaml_ptr<xaml_node> const& mc, xml_node& node) noexcept
{
    xaml_ptr<xaml_vector> props;
    XAML_RETURN_IF_FAILED(mc->get_properties(&props));
    if (!props)
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&props));
        XAML_RETURN_IF_FAILED(mc->set_properties(props.get()));
    }
    xaml_ptr<xaml_map> cprops;
    XAML_RETURN_IF_FAILED(mc->get_collection_properties(&cprops));
    if (!cprops)
    {
        xaml_ptr<xaml_hasher> hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(hasher.get(), &cprops));
        XAML_RETURN_IF_FAILED(mc->set_collection_properties(cprops.get()));
    }
    xaml_ptr<xaml_vector> events;
    XAML_RETURN_IF_FAILED(mc->get_events(&events));
    if (!events)
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&events));
        XAML_RETURN_IF_FAILED(mc->set_events(events.get()));
    }
    switch (node.type())
    {
    case node_type::element:
    {
        auto ns = node.namespace_uri();
        for (auto& attr : node.attributes())
        {
            auto attr_ns = attr.namespace_uri();
            auto attr_name = attr.local_name();
            if (attr_ns == xmlns_namespace::uri && attr_name == "xmlns")
                continue;
            else if (attr_ns == xmlns_namespace::uri)
                continue;
            else if (attr_ns.empty())
                attr_ns = ns;
            if (attr_ns == x_ns)
            {
                string_view attr_value = attr.value();
                if (attr_name == "name")
                {
                    xaml_ptr<xaml_string> attr_value_str;
                    XAML_RETURN_IF_FAILED(xaml_string_new_utf8(attr_value, &attr_value_str));
                    XAML_RETURN_IF_FAILED(mc->set_name(attr_value_str.get()));
                }
                else if (attr_name == "class")
                {
                    xaml_ptr<xaml_string> full_map_name;
                    XAML_RETURN_IF_FAILED(xaml_string_new_utf8(attr_value, &full_map_name));
                    XAML_RETURN_IF_FAILED(mc->set_map_class(full_map_name.get()));
                }
            }
            else
            {
                int32_t dm_index = attr_name.find_first_of('.');
                if (dm_index != string_view::npos)
                {
                    string_view class_name = attr_name.substr(0, dm_index);
                    string_view attach_prop_name = attr_name.substr(dm_index + 1);
                    xaml_ptr<xaml_reflection_info> info;
                    {

                        xaml_ptr<xaml_string> attr_ns_str;
                        XAML_RETURN_IF_FAILED(xaml_string_new_utf8(attr_ns, &attr_ns_str));
                        xaml_ptr<xaml_string> class_name_str;
                        XAML_RETURN_IF_FAILED(xaml_string_new_utf8(class_name, &class_name_str));
                        XAML_RETURN_IF_FAILED(ctx->get_type_by_namespace_name(attr_ns_str.get(), class_name_str.get(), &info));
                    }
                    xaml_ptr<xaml_type_info> t;
                    XAML_RETURN_IF_FAILED(info->query(&t));
                    xaml_ptr<xaml_string> include_file;
                    XAML_RETURN_IF_FAILED(t->get_include_file(&include_file));
                    if (include_file)
                        headers->append(include_file.get());
                    xaml_ptr<xaml_property_info> prop;
                    {
                        xaml_ptr<xaml_string> aprop_name_str;
                        XAML_RETURN_IF_FAILED(xaml_string_new_utf8(attach_prop_name, &aprop_name_str));
                        XAML_RETURN_IF_FAILED(t->get_property(aprop_name_str.get(), &prop));
                    }
                    bool can_write;
                    XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
                    if (can_write)
                    {
                        xaml_ptr<xaml_string> attr_value_str;
                        XAML_RETURN_IF_FAILED(xaml_string_new_utf8(attr.value(), &attr_value_str));
                        xaml_ptr<xaml_string_node> node;
                        XAML_RETURN_IF_FAILED(xaml_string_node_new(&node));
                        XAML_RETURN_IF_FAILED(node->set_value(attr_value_str.get()));
                        xaml_ptr<xaml_attribute_property> prop_item;
                        XAML_RETURN_IF_FAILED(xaml_attribute_property_new(t.get(), prop.get(), node.get(), &prop_item));
                        XAML_RETURN_IF_FAILED(props->append(prop_item.get()));
                    }
                }
                else
                {
                    xaml_ptr<xaml_type_info> type;
                    XAML_RETURN_IF_FAILED(mc->get_type(&type));
                    xaml_ptr<xaml_property_info> prop;
                    xaml_ptr<xaml_string> attr_name_str;
                    XAML_RETURN_IF_FAILED(xaml_string_new_utf8(attr_name, &attr_name_str));
                    XAML_RETURN_IF_FAILED(type->get_property(attr_name_str.get(), &prop));
                    bool can_write;
                    XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
                    if (can_write)
                    {
                        string_view attr_value = attr.value();
                        if (attr_value.front() == '{' && attr_value.back() == '}')
                        {
                            xaml_ptr<xaml_markup_node> ex;
                            XAML_RETURN_IF_FAILED(parse_markup(attr_value.substr(1, attr_value.length() - 2), &ex));
                            xaml_ptr<xaml_attribute_property> prop_item;
                            XAML_RETURN_IF_FAILED(xaml_attribute_property_new(type.get(), prop.get(), ex.get(), &prop_item));
                            XAML_RETURN_IF_FAILED(props->append(prop_item.get()));
                        }
                        else
                        {
                            xaml_ptr<xaml_string> attr_value_str;
                            XAML_RETURN_IF_FAILED(xaml_string_new_utf8(attr_value, &attr_value_str));
                            xaml_ptr<xaml_string_node> node;
                            XAML_RETURN_IF_FAILED(xaml_string_node_new(&node));
                            XAML_RETURN_IF_FAILED(node->set_value(attr_value_str.get()));
                            xaml_ptr<xaml_attribute_property> prop_item;
                            XAML_RETURN_IF_FAILED(xaml_attribute_property_new(type.get(), prop.get(), node.get(), &prop_item));
                            XAML_RETURN_IF_FAILED(props->append(prop_item.get()));
                        }
                    }
                    else
                    {
                        xaml_ptr<xaml_event_info> ev;
                        XAML_RETURN_IF_FAILED(type->get_event(attr_name_str.get(), &ev));
                        xaml_ptr<xaml_string> attr_value_str;
                        XAML_RETURN_IF_FAILED(xaml_string_new_utf8(attr.value(), &attr_value_str));
                        xaml_ptr<xaml_attribute_event> ev_item;
                        XAML_RETURN_IF_FAILED(xaml_attribute_event_new(ev.get(), attr_value_str.get(), &ev_item));
                        XAML_RETURN_IF_FAILED(events->append(ev_item.get()));
                    }
                }
            }
        }
        break;
    }
    case node_type::data:
    case node_type::cdata:
    {
        xaml_ptr<xaml_type_info> type;
        XAML_RETURN_IF_FAILED(mc->get_type(&type));
        xaml_ptr<xaml_default_property> def_attr;
        if (XAML_SUCCEEDED(type->get_attribute(&def_attr)))
        {
            xaml_ptr<xaml_string> prop_name;
            XAML_RETURN_IF_FAILED(def_attr->get_default_property(&prop_name));
            xaml_ptr<xaml_property_info> prop;
            XAML_RETURN_IF_FAILED(type->get_property(prop_name.get(), &prop));
            bool can_write;
            XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
            if (can_write)
            {
                xaml_ptr<xaml_string> node_value_str;
                XAML_RETURN_IF_FAILED(xaml_string_new_utf8(node.value(), &node_value_str));
                xaml_ptr<xaml_string_node> node;
                XAML_RETURN_IF_FAILED(xaml_string_node_new(&node));
                XAML_RETURN_IF_FAILED(node->set_value(node_value_str.get()));
                xaml_ptr<xaml_attribute_property> prop_item;
                XAML_RETURN_IF_FAILED(xaml_attribute_property_new(type.get(), prop.get(), node.get(), &prop_item));
                XAML_RETURN_IF_FAILED(props->append(prop_item.get()));
            }
        }
        break;
    }
    default:
        break;
    }
    xaml_ptr<xaml_type_info> type;
    XAML_RETURN_IF_FAILED(mc->get_type(&type));
    for (auto& c : node.nodes())
    {
        if (c.type() == node_type::element)
        {
            auto ns = c.namespace_uri();
            xaml_ptr<xaml_string> ns_str;
            XAML_RETURN_IF_FAILED(xaml_string_new_utf8(ns, &ns_str));
            auto name = c.local_name();
            int32_t dm_index = name.find_first_of('.');
            if (dm_index != string_view::npos)
            {
                string_view class_name = name.substr(0, dm_index);
                string_view prop_name = name.substr(dm_index + 1);
                xaml_ptr<xaml_reflection_info> info;
                {
                    xaml_ptr<xaml_string> class_name_str;
                    XAML_RETURN_IF_FAILED(xaml_string_new_utf8(class_name, &class_name_str));
                    XAML_RETURN_IF_FAILED(ctx->get_type_by_namespace_name(ns_str.get(), class_name_str.get(), &info));
                }
                xaml_ptr<xaml_type_info> t;
                XAML_RETURN_IF_FAILED(info->query(&t));
                xaml_ptr<xaml_string> include_file;
                XAML_RETURN_IF_FAILED(t->get_include_file(&include_file));
                if (include_file)
                    headers->append(include_file.get());
                xaml_ptr<xaml_node> child;
                XAML_RETURN_IF_FAILED(parse_impl(c, t, &child));
                xaml_ptr<xaml_property_info> prop;
                {
                    xaml_ptr<xaml_string> prop_name_str;
                    XAML_RETURN_IF_FAILED(xaml_string_new_utf8(prop_name, &prop_name_str));
                    XAML_RETURN_IF_FAILED(t->get_property(prop_name_str.get(), &prop));
                }
                bool can_write;
                XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
                if (can_write)
                {
                    xaml_ptr<xaml_attribute_property> prop_item;
                    XAML_RETURN_IF_FAILED(xaml_attribute_property_new(type.get(), prop.get(), child.get(), &prop_item));
                    XAML_RETURN_IF_FAILED(props->append(prop_item.get()));
                }
            }
            else
            {
                xaml_ptr<xaml_reflection_info> info;
                {
                    xaml_ptr<xaml_string> name_str;
                    XAML_RETURN_IF_FAILED(xaml_string_new_utf8(name, &name_str));
                    XAML_RETURN_IF_FAILED(ctx->get_type_by_namespace_name(ns_str.get(), name_str.get(), &info));
                }
                xaml_ptr<xaml_type_info> t;
                XAML_RETURN_IF_FAILED(info->query(&t));
                xaml_ptr<xaml_node> child;
                XAML_RETURN_IF_FAILED(parse_impl(c, t, &child));
                xaml_ptr<xaml_default_property> def_attr;
                if (XAML_SUCCEEDED(type->get_attribute(&def_attr)))
                {
                    xaml_ptr<xaml_string> prop_name;
                    XAML_RETURN_IF_FAILED(def_attr->get_default_property(&prop_name));
                    xaml_ptr<xaml_property_info> prop;
                    if (XAML_SUCCEEDED(type->get_property(prop_name.get(), &prop)))
                    {
                        bool can_write;
                        XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
                        if (can_write)
                        {
                            xaml_ptr<xaml_attribute_property> prop_item;
                            XAML_RETURN_IF_FAILED(xaml_attribute_property_new(type.get(), prop.get(), child.get(), &prop_item));
                            XAML_RETURN_IF_FAILED(props->append(prop_item.get()));
                        }
                    }
                    else
                    {
                        xaml_ptr<xaml_collection_property_info> info;
                        XAML_RETURN_IF_FAILED(type->get_collection_property(prop_name.get(), &info));
                        bool can_add;
                        XAML_RETURN_IF_FAILED(info->get_can_add(&can_add));
                        if (can_add)
                        {
                            xaml_ptr<xaml_string> name;
                            XAML_RETURN_IF_FAILED(info->get_name(&name));
                            xaml_ptr<xaml_attribute_collection_property> cprop;
                            xaml_ptr<xaml_vector> values;
                            xaml_ptr<xaml_object> item;
                            if (XAML_SUCCEEDED(cprops->lookup(name.get(), &item)))
                            {
                                XAML_RETURN_IF_FAILED(item->query(&cprop));
                            }
                            else
                            {
                                XAML_RETURN_IF_FAILED(xaml_vector_new(&values));
                                XAML_RETURN_IF_FAILED(xaml_attribute_collection_property_new(type.get(), info.get(), values.get(), &cprop));
                                bool replaced;
                                XAML_RETURN_IF_FAILED(cprops->insert(name.get(), item.get(), &replaced));
                            }
                            XAML_RETURN_IF_FAILED(values->append(child.get()));
                        }
                    }
                }
            }
        }
        else
        {
            XAML_RETURN_IF_FAILED(parse_members(mc, c));
        }
    }
    xaml_ptr<xaml_string> name;
    XAML_RETURN_IF_FAILED(mc->get_name(&name));
    if (!name)
    {
        XAML_RETURN_IF_FAILED(get_random_name(type, &name));
        XAML_RETURN_IF_FAILED(mc->set_name(name.get()));
    }
    return XAML_S_OK;
}

xaml_result parser_impl::parse_impl(xml_node& node, xaml_ptr<xaml_type_info> const& t, xaml_node** ptr) noexcept
{
    xaml_ptr<xaml_string> include_file;
    XAML_RETURN_IF_FAILED(t->get_include_file(&include_file));
    if (include_file)
        headers->append(include_file.get());
    xaml_ptr<xaml_node> mc;
    XAML_RETURN_IF_FAILED(xaml_node_new(&mc));
    XAML_RETURN_IF_FAILED(mc->set_type(t.get()));
    XAML_RETURN_IF_FAILED(parse_members(mc, node));
    return mc->query(ptr);
}

xaml_result parser_impl::parse(xaml_node** ptr) noexcept
{
    auto& root_nodes = doc.node().nodes();
    auto it = find_if(root_nodes.begin(), root_nodes.end(), [](xml_node& node) { return node.type() == node_type::element; });
    if (it == root_nodes.end()) return {};
    auto& root_node = *it;
    auto ns = root_node.namespace_uri();
    auto name = root_node.local_name();
    xaml_ptr<xaml_reflection_info> info;
    {
        xaml_ptr<xaml_string> ns_str;
        XAML_RETURN_IF_FAILED(xaml_string_new_utf8(ns, &ns_str));
        xaml_ptr<xaml_string> name_str;
        XAML_RETURN_IF_FAILED(xaml_string_new_utf8(name, &name_str));
        XAML_RETURN_IF_FAILED(ctx->get_type_by_namespace_name(ns_str.get(), name_str.get(), &info));
    }
    xaml_ptr<xaml_type_info> t;
    XAML_RETURN_IF_FAILED(info->query(&t));
    return parse_impl(root_node, t, ptr);
}
