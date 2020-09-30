#define XAML_RAISE_LEVEL xaml_result_raise_warning

#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_document.hpp>
#include <sf/sformat.hpp>
#include <sstream>
#include <xaml/internal/stream.hpp>
#include <xaml/parser/parser.h>

using namespace std;
using namespace rapidxml;

static xaml_result get_random_name(xaml_ptr<xaml_type_info> const& ref, xaml_string** ptr) noexcept
{
    static size_t index = 0;
    xaml_ptr<xaml_string> name;
    XAML_RETURN_IF_FAILED(ref->get_name(&name));
    try
    {
        return xaml_string_new(sf::sprint(U("__{}__{}"), name, index++), ptr);
    }
    XAML_CATCH_RETURN()
}

static xaml_result to_xaml_result() noexcept
try
{
    throw;
}
catch (rapidxml::parse_error const& e)
{
    string msg = sf::sprint(U("{}:{}:{}:{}"), e.what(), e.where(), e.row(), e.col());
    xaml_result_raise_message(XAML_E_FAIL, xaml_result_raise_error, msg.c_str());
    return XAML_E_FAIL;
}
XAML_CATCH_RETURN()

struct parser_impl
{
    xaml_ptr<xaml_meta_context> ctx{ nullptr };
    xaml_ptr<xaml_vector<xaml_string>> headers{};
    xml_document doc{};

    xaml_result load_string(string_view s) noexcept
    try
    {
        doc.load_string(s);
        return XAML_S_OK;
    }
    catch (...)
    {
        return to_xaml_result();
    }

    xaml_result load_buffer(xaml_ptr<xaml_buffer> const& buffer) noexcept
    {
        uint8_t* data;
        XAML_RETURN_IF_FAILED(buffer->get_data(&data));
        int32_t size;
        XAML_RETURN_IF_FAILED(buffer->get_size(&size));
        return load_string(string_view((char const*)data, (size_t)size));
    }

    xaml_result load_stream(istream& stream) noexcept
    try
    {
        doc.load_stream(stream);
        return XAML_S_OK;
    }
    catch (...)
    {
        return to_xaml_result();
    }

    xaml_result load_stream(FILE* stream) noexcept
    {
        return cfile_istream_invoke<char>([this](istream& stream) noexcept -> xaml_result { return load_stream(stream); }, stream);
    }

    xaml_result init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&headers));
        return XAML_S_OK;
    }

    xaml_result parse_markup(string_view value, xaml_markup_node** ptr) noexcept;
    xaml_result parse_members(xaml_ptr<xaml_node> const& mc, xml_node& node) noexcept;
    xaml_result parse_impl(xml_node& node, xaml_node** ptr) noexcept;
    xaml_result parse_impl(xml_node& node, xaml_ptr<xaml_type_info> const& t, xaml_node** ptr) noexcept;
    xaml_result parse(xaml_node** ptr) noexcept;

    xaml_result add_include_file(xaml_reflection_info* info) noexcept
    {
        xaml_ptr<xaml_string> include_file;
        XAML_RETURN_IF_FAILED(info->get_include_file(&include_file));
        if (include_file)
            XAML_RETURN_IF_FAILED(headers->append(include_file));
        return XAML_S_OK;
    }
};

xaml_result parser_impl::parse_markup(string_view value, xaml_markup_node** ptr) noexcept
{
    string_view ns, name;
    size_t sep_index = 0;
    size_t i = 0;
    // The start of `value` should be `foo ` or `ns:foo `,
    // find `:` or space to determine type name and namespace.
    for (; i < value.length(); i++)
    {
        if (ns.empty() && value[i] == ':')
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
    // If namespace is empty, use default one.
    if (ns.empty()) ns = "xaml";
    if (name.empty()) name = value.substr(sep_index);
    // Find the type
    xaml_ptr<xaml_reflection_info> info;
    {
        xaml_ptr<xaml_string> ns_str;
        XAML_RETURN_IF_FAILED(xaml_string_new(ns, &ns_str));
        xaml_ptr<xaml_string> name_str;
        XAML_RETURN_IF_FAILED(xaml_string_new(name, &name_str));
        XAML_RETURN_IF_FAILED(ctx->get_type_by_namespace_name(ns_str, name_str, &info));
    }
    XAML_RETURN_IF_FAILED(add_include_file(info));
    xaml_ptr<xaml_type_info> t;
    XAML_RETURN_IF_FAILED(info->query(&t));
    // Give it a random name, because it won't have real name.
    xaml_ptr<xaml_string> node_name;
    XAML_RETURN_IF_FAILED(get_random_name(t, &node_name));
    // Initialize the markup node.
    xaml_ptr<xaml_markup_node> node;
    XAML_RETURN_IF_FAILED(xaml_markup_node_new(&node));
    XAML_RETURN_IF_FAILED(node->set_type(t));
    XAML_RETURN_IF_FAILED(node->set_name(node_name));
    // The properties of the node, assign at last.
    xaml_ptr<xaml_vector<xaml_attribute_property>> props;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&props));
    while (i < value.length())
    {
        // Skip spaces
        while (i < value.length() && isspace(value[i])) i++;
        // Find `,` or `=`;
        // the format should be `def_prop_value, prop=value, prop=value`
        size_t start_index = i;
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
        // If not find, then it is default property
        if (i == value.length()) i = start_index;
        // If it is default property, the name should be empty now
        string_view prop_name = value.substr(start_index, i - start_index);
        // value[i] could be only `=` or the start of (default) value
        while (i < value.length() && value[i] == '=') i++;
        // So `start_index` is at the start of value now
        start_index = i;
        // Find the end of value then
        while (i < value.length() && value[i] != ',') i++;
        // So we get the value
        string_view prop_value = value.substr(start_index, i - start_index);
        // Bump i for next loop
        while (i < value.length() && value[i] == ',') i++;
        // Get property name, especially for default one
        xaml_ptr<xaml_string> prop_name_str;
        if (prop_name.empty())
        {
            xaml_ptr<xaml_default_property> def_attr;
            XAML_RETURN_IF_FAILED(t->get_attribute(&def_attr));
            XAML_RETURN_IF_FAILED(def_attr->get_default_property(&prop_name_str));
        }
        else
        {
            XAML_RETURN_IF_FAILED(xaml_string_new(prop_name, &prop_name_str));
        }
        // Find the property
        xaml_ptr<xaml_property_info> prop;
        XAML_RETURN_IF_FAILED(t->get_property(prop_name_str, &prop));
        bool can_write;
        XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
        if (can_write)
        {
            xaml_ptr<xaml_node_base> node_value;
            // The value maybe another markup node
            if (prop_value.starts_with('{') && prop_value.ends_with('}'))
            {
                xaml_ptr<xaml_markup_node> ex;
                XAML_RETURN_IF_FAILED(parse_markup(prop_value.substr(1, prop_value.length() - 2), &ex));
                node_value = ex;
            }
            // or a string
            else
            {
                xaml_ptr<xaml_string> prop_value_str;
                XAML_RETURN_IF_FAILED(xaml_string_new(prop_value, &prop_value_str));
                xaml_ptr<xaml_string_node> node_str_value;
                XAML_RETURN_IF_FAILED(xaml_string_node_new(&node_str_value));
                XAML_RETURN_IF_FAILED(node_str_value->set_value(prop_value_str));
                node_value = node_str_value;
            }
            xaml_ptr<xaml_attribute_property> prop_item;
            XAML_RETURN_IF_FAILED(xaml_attribute_property_new(t, prop, node_value, &prop_item));
            XAML_RETURN_IF_FAILED(props->append(prop_item));
        }
    }
    XAML_RETURN_IF_FAILED(node->set_properties(props));
    return node->query(ptr);
}

static constexpr string_view x_ns{ "https://github.com/Berrysoft/XamlCpp/xaml/" };

// Add string node to properties vector
static xaml_result props_add_string_property(xaml_ptr<xaml_vector<xaml_attribute_property>> const& props, xaml_ptr<xaml_type_info> const& t, xaml_ptr<xaml_property_info> const& prop, string_view value) noexcept
{
    xaml_ptr<xaml_string> attr_value_str;
    XAML_RETURN_IF_FAILED(xaml_string_new(value, &attr_value_str));
    xaml_ptr<xaml_string_node> node_value;
    XAML_RETURN_IF_FAILED(xaml_string_node_new(&node_value));
    XAML_RETURN_IF_FAILED(node_value->set_value(attr_value_str));
    xaml_ptr<xaml_attribute_property> prop_item;
    XAML_RETURN_IF_FAILED(xaml_attribute_property_new(t, prop, node_value, &prop_item));
    return props->append(prop_item);
}

// Get values vector from a collection proeperty
static xaml_result get_cprop_values(xaml_ptr<xaml_map<xaml_string, xaml_attribute_collection_property>> const& cprops, xaml_ptr<xaml_collection_property_info> const& cprop, xaml_ptr<xaml_type_info> const& type, xaml_vector<xaml_node>** pvalues) noexcept
{
    xaml_ptr<xaml_attribute_collection_property> cprop_item;
    xaml_ptr<xaml_vector<xaml_node>> values;
    xaml_ptr<xaml_string> prop_name_str;
    XAML_RETURN_IF_FAILED(cprop->get_name(&prop_name_str));
    if (XAML_SUCCEEDED(cprops->lookup(prop_name_str, &cprop_item)))
    {
        XAML_RETURN_IF_FAILED(cprop_item->get_values(&values));
    }
    else
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&values));
        XAML_RETURN_IF_FAILED(xaml_attribute_collection_property_new(type, cprop, values, &cprop_item));
        XAML_RETURN_IF_FAILED(cprops->insert(prop_name_str, cprop_item, nullptr));
    }
    return values.query(pvalues);
}

xaml_result parser_impl::parse_members(xaml_ptr<xaml_node> const& mc, xml_node& node) noexcept
{
    xaml_ptr<xaml_map<xaml_string, xaml_node>> reses;
    XAML_RETURN_IF_FAILED(mc->get_resources(&reses));
    if (!reses)
    {
        xaml_ptr<xaml_hasher<xaml_string>> hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new(hasher.get(), &reses));
        XAML_RETURN_IF_FAILED(mc->set_resources(reses));
    }
    xaml_ptr<xaml_vector<xaml_attribute_property>> props;
    XAML_RETURN_IF_FAILED(mc->get_properties(&props));
    if (!props)
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&props));
        XAML_RETURN_IF_FAILED(mc->set_properties(props));
    }
    xaml_ptr<xaml_map<xaml_string, xaml_attribute_collection_property>> cprops;
    XAML_RETURN_IF_FAILED(mc->get_collection_properties(&cprops));
    if (!cprops)
    {
        xaml_ptr<xaml_hasher<xaml_string>> hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new(hasher.get(), &cprops));
        XAML_RETURN_IF_FAILED(mc->set_collection_properties(cprops));
    }
    xaml_ptr<xaml_vector<xaml_attribute_event>> events;
    XAML_RETURN_IF_FAILED(mc->get_events(&events));
    if (!events)
    {
        XAML_RETURN_IF_FAILED(xaml_vector_new(&events));
        XAML_RETURN_IF_FAILED(mc->set_events(events));
    }

    switch (node.type())
    {
    case node_type::element:
    {
        auto ns = node.namespace_uri();
        // There's a case, when using attached properties,
        // the name of the class should be specified.
        // And now the default namespace of the specified class
        // is the same as the namespace of the node.
        for (auto& attr : node.attributes())
        {
            auto attr_ns = attr.namespace_uri();
            auto attr_name = attr.local_name();
            // xmlns is not property
            if ((attr_ns == xml_namespace::uri && attr_name == "xmlns") || (attr_ns == xmlns_namespace::uri))
                continue;
            // When the namespace of property is empty, use default one
            else if (attr_ns.empty())
                attr_ns = ns;
            // Special namespace: x
            if (attr_ns == x_ns)
            {
                string_view attr_value = attr.value();
                xaml_ptr<xaml_string> attr_value_str;
                XAML_RETURN_IF_FAILED(xaml_string_new(attr_value, &attr_value_str));
                if (attr_name == "name")
                {
                    XAML_RETURN_IF_FAILED(mc->set_name(attr_value_str));
                }
                else if (attr_name == "key")
                {
                    XAML_RETURN_IF_FAILED(mc->set_key(attr_value_str));
                }
            }
            else
            {
                // Determine if it is attached property.
                size_t dm_index = attr_name.find_first_of('.');
                if (dm_index != string_view::npos)
                {
                    // Find class
                    string_view class_name = attr_name.substr(0, dm_index);
                    string_view attach_prop_name = attr_name.substr(dm_index + 1);
                    xaml_ptr<xaml_reflection_info> info;
                    {
                        xaml_ptr<xaml_string> attr_ns_str;
                        XAML_RETURN_IF_FAILED(xaml_string_new(attr_ns, &attr_ns_str));
                        xaml_ptr<xaml_string> class_name_str;
                        XAML_RETURN_IF_FAILED(xaml_string_new(class_name, &class_name_str));
                        XAML_RETURN_IF_FAILED(ctx->get_type_by_namespace_name(attr_ns_str, class_name_str, &info));
                    }
                    XAML_RETURN_IF_FAILED(add_include_file(info));
                    xaml_ptr<xaml_type_info> t;
                    XAML_RETURN_IF_FAILED(info->query(&t));
                    // Find property
                    xaml_ptr<xaml_property_info> prop;
                    {
                        xaml_ptr<xaml_string> aprop_name_str;
                        XAML_RETURN_IF_FAILED(xaml_string_new(attach_prop_name, &aprop_name_str));
                        XAML_RETURN_IF_FAILED(t->get_property(aprop_name_str, &prop));
                    }
                    bool can_write;
                    XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
                    if (can_write)
                    {
                        // Support string value only for attached proeprty up to now
                        XAML_RETURN_IF_FAILED(props_add_string_property(props, t, prop, attr.value()));
                    }
                }
                else
                {
                    // Find property from type of node
                    xaml_ptr<xaml_type_info> type;
                    XAML_RETURN_IF_FAILED(mc->get_type(&type));
                    xaml_ptr<xaml_property_info> prop;
                    xaml_ptr<xaml_string> attr_name_str;
                    XAML_RETURN_IF_FAILED(xaml_string_new(attr_name, &attr_name_str));
                    if (XAML_SUCCEEDED(type->get_property(attr_name_str, &prop)))
                    {
                        bool can_write;
                        XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
                        if (can_write)
                        {
                            string_view attr_value = attr.value();
                            // Support markup extensions
                            if (attr_value.starts_with('{') && attr_value.ends_with('}'))
                            {
                                xaml_ptr<xaml_markup_node> ex;
                                XAML_RETURN_IF_FAILED(parse_markup(attr_value.substr(1, attr_value.length() - 2), &ex));
                                xaml_ptr<xaml_attribute_property> prop_item;
                                XAML_RETURN_IF_FAILED(xaml_attribute_property_new(type, prop, ex, &prop_item));
                                XAML_RETURN_IF_FAILED(props->append(prop_item));
                            }
                            else
                            {
                                XAML_RETURN_IF_FAILED(props_add_string_property(props, type, prop, attr_value));
                            }
                        }
                    }
                    else
                    {
                        // If it is not a property, it should be an event
                        xaml_ptr<xaml_event_info> ev;
                        XAML_RETURN_IF_FAILED(type->get_event(attr_name_str, &ev));
                        xaml_ptr<xaml_string> attr_value_str;
                        XAML_RETURN_IF_FAILED(xaml_string_new(attr.value(), &attr_value_str));
                        xaml_ptr<xaml_attribute_event> ev_item;
                        XAML_RETURN_IF_FAILED(xaml_attribute_event_new(ev, attr_value_str, &ev_item));
                        XAML_RETURN_IF_FAILED(events->append(ev_item));
                    }
                }
            }
        }
        break;
    }
    // Text nodes
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
            XAML_RETURN_IF_FAILED(type->get_property(prop_name, &prop));
            bool can_write;
            XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
            if (can_write)
            {
                XAML_RETURN_IF_FAILED(props_add_string_property(props, type, prop, node.value()));
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
        // If the child node is a new element, construct it and parse its members;
        // otherwise, parse members for the current node
        if (c.type() == node_type::element)
        {
            auto ns = c.namespace_uri();
            xaml_ptr<xaml_string> ns_str;
            XAML_RETURN_IF_FAILED(xaml_string_new(ns, &ns_str));
            auto name = c.local_name();
            size_t dm_index = name.find_first_of('.');
            // This is a property
            if (dm_index != string_view::npos)
            {
                string_view class_name = name.substr(0, dm_index);
                string_view prop_name = name.substr(dm_index + 1);
                xaml_ptr<xaml_reflection_info> info;
                {
                    xaml_ptr<xaml_string> class_name_str;
                    XAML_RETURN_IF_FAILED(xaml_string_new(class_name, &class_name_str));
                    XAML_RETURN_IF_FAILED(ctx->get_type_by_namespace_name(ns_str, class_name_str, &info));
                }
                XAML_RETURN_IF_FAILED(add_include_file(info));
                xaml_ptr<xaml_type_info> t;
                XAML_RETURN_IF_FAILED(info->query(&t));
                // Deal with resources
                if (prop_name == "resources")
                {
                    for (auto& cnode : c.nodes())
                    {
                        xaml_ptr<xaml_node> child;
                        XAML_RETURN_IF_FAILED(parse_impl(cnode, &child));
                        xaml_ptr<xaml_string> key;
                        XAML_RETURN_IF_FAILED(child->get_key(&key));
                        bool replaced;
                        XAML_RETURN_IF_FAILED(reses->insert(key, child, &replaced));
                    }
                }
                else
                {
                    xaml_ptr<xaml_string> prop_name_str;
                    XAML_RETURN_IF_FAILED(xaml_string_new(prop_name, &prop_name_str));
                    // If it is a property, add child node
                    xaml_ptr<xaml_property_info> prop;
                    if (XAML_SUCCEEDED(t->get_property(prop_name_str, &prop)))
                    {
                        bool can_write;
                        XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
                        if (can_write)
                        {
                            xml_node& cnode = c.nodes().front();
                            xaml_ptr<xaml_node> child;
                            XAML_RETURN_IF_FAILED(parse_impl(cnode, &child));
                            xaml_ptr<xaml_attribute_property> prop_item;
                            XAML_RETURN_IF_FAILED(xaml_attribute_property_new(type, prop, child, &prop_item));
                            XAML_RETURN_IF_FAILED(props->append(prop_item));
                        }
                    }
                    else
                    {
                        // Or it is a collection property
                        xaml_ptr<xaml_collection_property_info> cprop;
                        if (XAML_SUCCEEDED(t->get_collection_property(prop_name_str, &cprop)))
                        {
                            bool can_add;
                            XAML_RETURN_IF_FAILED(cprop->get_can_add(&can_add));
                            if (can_add)
                            {
                                xaml_ptr<xaml_vector<xaml_node>> values;
                                XAML_RETURN_IF_FAILED(get_cprop_values(cprops, cprop, type, &values));
                                for (auto& cnode : c.nodes())
                                {
                                    xaml_ptr<xaml_node> child;
                                    XAML_RETURN_IF_FAILED(parse_impl(cnode, &child));
                                    XAML_RETURN_IF_FAILED(values->append(child));
                                }
                            }
                        }
                    }
                }
            }
            // A new node
            else
            {
                xaml_ptr<xaml_reflection_info> info;
                {
                    xaml_ptr<xaml_string> name_str;
                    XAML_RETURN_IF_FAILED(xaml_string_new(name, &name_str));
                    XAML_RETURN_IF_FAILED(ctx->get_type_by_namespace_name(ns_str, name_str, &info));
                }
                xaml_ptr<xaml_type_info> t;
                XAML_RETURN_IF_FAILED(info->query(&t));
                // Parse the child
                xaml_ptr<xaml_node> child;
                XAML_RETURN_IF_FAILED(parse_impl(c, t, &child));
                xaml_ptr<xaml_default_property> def_attr;
                if (XAML_SUCCEEDED(type->get_attribute(&def_attr)))
                {
                    xaml_ptr<xaml_string> prop_name;
                    XAML_RETURN_IF_FAILED(def_attr->get_default_property(&prop_name));
                    xaml_ptr<xaml_property_info> prop;
                    if (XAML_SUCCEEDED(type->get_property(prop_name, &prop)))
                    {
                        bool can_write;
                        XAML_RETURN_IF_FAILED(prop->get_can_write(&can_write));
                        if (can_write)
                        {
                            xaml_ptr<xaml_attribute_property> prop_item;
                            XAML_RETURN_IF_FAILED(xaml_attribute_property_new(type, prop, child, &prop_item));
                            XAML_RETURN_IF_FAILED(props->append(prop_item));
                        }
                    }
                    else
                    {
                        xaml_ptr<xaml_collection_property_info> info2;
                        XAML_RETURN_IF_FAILED(type->get_collection_property(prop_name, &info2));
                        bool can_add;
                        XAML_RETURN_IF_FAILED(info2->get_can_add(&can_add));
                        if (can_add)
                        {
                            xaml_ptr<xaml_vector<xaml_node>> values;
                            XAML_RETURN_IF_FAILED(get_cprop_values(cprops, info2, type, &values));
                            XAML_RETURN_IF_FAILED(values->append(child));
                        }
                    }
                }
            }
        }
        // So it is a child node
        else
        {
            XAML_RETURN_IF_FAILED(parse_members(mc, c));
        }
    }
    // Check if it already has a name
    xaml_ptr<xaml_string> name;
    XAML_RETURN_IF_FAILED(mc->get_name(&name));
    if (!name)
    {
        XAML_RETURN_IF_FAILED(get_random_name(type, &name));
        XAML_RETURN_IF_FAILED(mc->set_name(name));
    }
    return XAML_S_OK;
}

xaml_result parser_impl::parse_impl(xml_node& node, xaml_node** ptr) noexcept
{
    auto ns = node.namespace_uri();
    auto name = node.local_name();
    xaml_ptr<xaml_reflection_info> info;
    {
        xaml_ptr<xaml_string> ns_str;
        XAML_RETURN_IF_FAILED(xaml_string_new(ns, &ns_str));
        xaml_ptr<xaml_string> name_str;
        XAML_RETURN_IF_FAILED(xaml_string_new(name, &name_str));
        XAML_RETURN_IF_FAILED(ctx->get_type_by_namespace_name(ns_str, name_str, &info));
    }
    xaml_ptr<xaml_type_info> t;
    XAML_RETURN_IF_FAILED(info->query(&t));
    return parse_impl(node, t, ptr);
}

xaml_result parser_impl::parse_impl(xml_node& node, xaml_ptr<xaml_type_info> const& t, xaml_node** ptr) noexcept
{
    XAML_RETURN_IF_FAILED(add_include_file(t));
    xaml_ptr<xaml_node> mc;
    XAML_RETURN_IF_FAILED(xaml_node_new(&mc));
    XAML_RETURN_IF_FAILED(mc->set_type(t));
    XAML_RETURN_IF_FAILED(parse_members(mc, node));
    return mc->query(ptr);
}

xaml_result parser_impl::parse(xaml_node** ptr) noexcept
{
    auto& root_nodes = doc.node().nodes();
    auto it = find_if(root_nodes.begin(), root_nodes.end(), [](xml_node& node) noexcept { return node.type() == node_type::element; });
    if (it == root_nodes.end()) return {};
    auto& root_node = *it;
    return parse_impl(root_node, ptr);
}

static xaml_result XAML_CALL xaml_parse_parse_impl(parser_impl& parser, xaml_meta_context* ctx, xaml_node** ptr, xaml_vector_view<xaml_string>** pheaders) noexcept
{
    parser.ctx = ctx;
    XAML_RETURN_IF_FAILED(parser.parse(ptr));
    return parser.headers->query(pheaders);
}

xaml_result XAML_CALL xaml_parser_parse_string(xaml_meta_context* ctx, xaml_string* str, xaml_node** ptr, xaml_vector_view<xaml_string>** pheaders) noexcept
{
    parser_impl parser{};
    XAML_RETURN_IF_FAILED(parser.init());
    string_view data;
    XAML_RETURN_IF_FAILED(to_string_view(str, &data));
    XAML_RETURN_IF_FAILED(parser.load_string(data));
    return xaml_parse_parse_impl(parser, ctx, ptr, pheaders);
}

xaml_result XAML_CALL xaml_parser_parse_buffer(xaml_meta_context* ctx, xaml_buffer* buffer, xaml_node** ptr, xaml_vector_view<xaml_string>** pheaders) noexcept
{
    parser_impl parser{};
    XAML_RETURN_IF_FAILED(parser.init());
    XAML_RETURN_IF_FAILED(parser.load_buffer(buffer));
    return xaml_parse_parse_impl(parser, ctx, ptr, pheaders);
}

xaml_result XAML_CALL xaml_parser_parse_stream(xaml_meta_context* ctx, FILE* stream, xaml_node** ptr, xaml_vector_view<xaml_string>** pheaders) noexcept
{
    parser_impl parser{};
    XAML_RETURN_IF_FAILED(parser.init());
    XAML_RETURN_IF_FAILED(parser.load_stream(stream));
    return xaml_parse_parse_impl(parser, ctx, ptr, pheaders);
}

xaml_result XAML_CALL xaml_parser_parse_stream(xaml_meta_context* ctx, istream& stream, xaml_node** ptr, xaml_vector_view<xaml_string>** pheaders) noexcept
{
    parser_impl parser{};
    XAML_RETURN_IF_FAILED(parser.init());
    XAML_RETURN_IF_FAILED(parser.load_stream(stream));
    return xaml_parse_parse_impl(parser, ctx, ptr, pheaders);
}
