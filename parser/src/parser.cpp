#include <fstream>
#include <iostream>
#include <rapidxml/xml_attribute.hpp>
#include <rapidxml/xml_document.hpp>
#include <sstream>
#include <vector>
#include <xaml/markup/binding.hpp>
#include <xaml/meta/meta.hpp>
#include <xaml/parser.hpp>

using namespace std;
using namespace std::filesystem;
using namespace rapidxml;

namespace xaml
{
    void init_parser(meta_context& ctx) noexcept
    {
        register_class<binding>(ctx);
        REGISTER_ENUM(xaml, binding_mode, "xaml/markup/binding.hpp");
    }

    static string get_no_default_constructor_error(reflection_info const* t)
    {
        auto& [ns, name] = t->get_type_name();
        ostringstream oss;
        oss << "Type \"" << ns << "::" << name << "\" doesn't have a default constructor.";
        return oss.str();
    }

    xaml_no_default_constructor::xaml_no_default_constructor(reflection_info const* t) : logic_error(get_no_default_constructor_error(t))
    {
    }

    static string get_bad_type_error(string_view ns, string_view name)
    {
        ostringstream oss;
        oss << "Cannot find type named \"" << name << "\" in namespace \"" << ns << "\".";
        return oss.str();
    }

    xaml_bad_type::xaml_bad_type(string_view ns, string_view name) : logic_error(get_bad_type_error(ns, name))
    {
    }

    static string get_xaml_no_member(reflection_info const* type, string_view name)
    {
        auto& [ns, n] = type->get_type_name();
        ostringstream oss;
        oss << "There's no member named " << name << " in type \"" << ns << "::" << n << "\".";
        return oss.str();
    }

    xaml_no_member::xaml_no_member(reflection_info const* type, string_view name) : logic_error(get_xaml_no_member(type, name))
    {
    }

    static ostream& write_valid_name(ostream& stream, string_view name)
    {
        for (char c : name)
        {
            if (isdigit(c) || isalpha(c))
                stream << c;
            else
                stream << '_';
        }
        return stream;
    }

    static string get_random_name(reflection_info const* ref)
    {
        static size_t index = 0;
        auto& [ns, name] = ref->get_type_name();
        ostringstream oss;
        write_valid_name(oss << "__", ns) << "__" << name << "__" << index++;
        return oss.str();
    }

    struct parser_impl
    {
        meta_context* ctx{ nullptr };
        string buffer;
        set<string> headers{};
        xml_document doc{};

        ~parser_impl() {}

        void load_file(path const& p)
        {
            ifstream stream{ p };
            buffer.assign(istreambuf_iterator<char>(stream), istreambuf_iterator<char>{});
            doc.parse(buffer.data());
        }

        void load_string(string_view s)
        {
            buffer.assign(s);
            doc.parse(buffer.data());
        }

        markup_node parse_markup(string_view value);
        void parse_members(xaml_node& mc, xml_node& node);
        xaml_node parse_impl(xml_node& node, reflection_info const* t);
        xaml_node parse();
    };

    markup_node parser_impl::parse_markup(string_view value)
    {
        string_view ns, name;
        size_t sep_index = 0;
        size_t i = 0;
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
        auto t = ctx->get_type(ns, name);
        if (t)
        {
            headers.emplace(t->get_include_file());
            markup_node node{ t, get_random_name(t) };
            while (i < value.length())
            {
                while (i < value.length() && isspace(value[i])) i++;
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
                string_view prop_name = value.substr(start_index, i - start_index);
                while (i < value.length() && value[i] == '=') i++;
                start_index = i;
                for (; i < value.length(); i++)
                {
                    if (value[i] == ',') break;
                }
                string_view prop_value = value.substr(start_index, i - start_index);
                while (i < value.length() && value[i] == ',') i++;
                if (prop_name.empty())
                {
                    auto def_attr = t->get_attribute<default_property>();
                    if (def_attr)
                    {
                        prop_name = def_attr->get_property_name();
                    }
                }
                auto prop = t->get_property(prop_name);
                if (prop && prop->can_write())
                {
                    node.properties.push_back({ t, prop, (string)prop_value });
                }
                else
                {
                    throw xaml_no_member(t, prop_name);
                }
            }
            return node;
        }
        throw xaml_bad_type(ns, name);
    }

    static constexpr string_view x_ns{ "https://github.com/Berrysoft/XamlCpp/xaml/" };

    void parser_impl::parse_members(xaml_node& mc, xml_node& node)
    {
        switch (node.type())
        {
        case node_type::element:
        {
            auto ns = node.namespace_uri();
            auto name = node.name();
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
                        mc.name = attr_value;
                    }
                    else if (attr_name == "class")
                    {
                        auto index = attr_value.find_last_of(':');
                        if (index != string_view::npos)
                        {
                            if (index > 0 && attr_value[index - 1] == ':' && index + 1 < attr_value.length())
                            {
                                string_view map_ns = attr_value.substr(0, index - 1);
                                string_view map_name = attr_value.substr(index + 1);
                                mc.map_class = make_optional(make_tuple<string, string>((string)map_ns, (string)map_name));
                            }
                        }
                        else
                        {
                            mc.map_class = make_optional(make_tuple<string, string>({}, (string)attr_value));
                        }
                    }
                }
                else
                {
                    size_t dm_index = attr_name.find_first_of('.');
                    if (dm_index != string_view::npos)
                    {
                        string_view class_name = attr_name.substr(0, dm_index);
                        string_view attach_prop_name = attr_name.substr(dm_index + 1);
                        auto t = ctx->get_type(attr_ns, class_name);
                        if (t)
                        {
                            headers.emplace(t->get_include_file());
                            auto prop = t->get_property(attach_prop_name);
                            if (prop && prop->can_write())
                            {
                                string_view attr_value = attr.value();
                                mc.properties.push_back({ t, prop, (string)attr_value });
                            }
                        }
                        else
                        {
                            throw xaml_bad_type(attr_ns, class_name);
                        }
                    }
                    else
                    {
                        auto prop = mc.type->get_property(attr_name);
                        if (prop && prop->can_write())
                        {
                            string_view attr_value = attr.value();
                            if (attr_value.front() == '{' && attr_value.back() == '}')
                            {
                                auto ex = parse_markup(attr_value.substr(1, attr_value.length() - 2));
                                mc.properties.push_back({ mc.type, prop, ex });
                            }
                            else
                            {
                                mc.properties.push_back({ mc.type, prop, (string)attr_value });
                            }
                        }
                        else
                        {
                            auto ev = mc.type->get_event(attr_name);
                            if (ev && ev->can_add())
                            {
                                string_view attr_value = attr.value();
                                mc.events.push_back({ ev, (string)attr_value });
                            }
                            else
                            {
                                throw xaml_no_member(mc.type, attr_name);
                            }
                        }
                    }
                }
            }
            break;
        }
        case node_type::data:
        case node_type::cdata:
        {
            auto def_attr = mc.type->get_attribute<default_property>();
            if (def_attr)
            {
                string_view prop_name = def_attr->get_property_name();
                auto prop = mc.type->get_property(prop_name);
                if (prop && prop->can_write())
                {
                    mc.properties.push_back({ mc.type, prop, (string)node.value() });
                }
                else
                {
                    throw xaml_no_member(mc.type, prop_name);
                }
            }
            break;
        }
        default:
            break;
        }
        for (auto& c : node.nodes())
        {
            if (c.type() == node_type::element)
            {
                auto ns = c.namespace_uri();
                auto name = c.local_name();
                size_t dm_index = name.find_first_of('.');
                if (dm_index != string_view::npos)
                {
                    string_view class_name = name.substr(0, dm_index);
                    string_view prop_name = name.substr(dm_index + 1);
                    auto t = ctx->get_type(ns, class_name);
                    if (t)
                    {
                        headers.emplace(t->get_include_file());
                        auto child = parse_impl(c, t);
                        if (mc.type->get_type() == t->get_type())
                        {
                            auto prop = mc.type->get_property(prop_name);
                            if (prop && prop->can_write())
                            {
                                mc.properties.push_back({ mc.type, prop, move(child) });
                            }
                        }
                        else
                        {
                            auto prop = t->get_property(prop_name);
                            if (prop && prop->can_write())
                            {
                                mc.properties.push_back({ t, prop, move(child) });
                            }
                        }
                    }
                    else
                    {
                        throw xaml_bad_type(ns, class_name);
                    }
                }
                else
                {
                    auto t = ctx->get_type(ns, name);
                    if (t)
                    {
                        auto child = parse_impl(c, t);
                        auto def_attr = mc.type->get_attribute<default_property>();
                        if (def_attr)
                        {
                            string_view prop_name = def_attr->get_property_name();
                            auto prop = mc.type->get_property(prop_name);
                            if (prop && prop->can_write())
                            {
                                mc.properties.push_back({ mc.type, prop, move(child) });
                            }
                            else
                            {
                                auto info = mc.type->get_collection_property(prop_name);
                                if (info && info->can_add())
                                {
                                    auto& prop = mc.collection_properties[(string)info->name()];
                                    prop.host_type = mc.type;
                                    prop.info = info;
                                    prop.values.push_back(move(child));
                                }
                            }
                        }
                    }
                    else
                    {
                        throw xaml_bad_type(ns, name);
                    }
                }
            }
            else
            {
                parse_members(mc, c);
            }
        }
        if (mc.name.empty())
        {
            mc.name = get_random_name(mc.type);
        }
    }

    xaml_node parser_impl::parse_impl(xml_node& node, reflection_info const* t)
    {
        headers.emplace(t->get_include_file());
        xaml_node mc{ t };
        parse_members(mc, node);
        return mc;
    }

    xaml_node parser_impl::parse()
    {
        xml_node& root_node = doc.node().nodes().front();
        auto ns = root_node.namespace_uri();
        auto name = root_node.local_name();
        auto t = ctx->get_type(ns, name);
        if (t)
        {
            return parse_impl(root_node, t);
        }
        else
        {
            throw xaml_bad_type(ns, name);
        }
    }

    static tuple<xaml_node, set<string>> parse_impl(meta_context& ctx, parser_impl& impl)
    {
        impl.ctx = &ctx;
        xaml_node result = impl.parse();
        return make_tuple(move(result), move(impl.headers));
    }

    tuple<xaml_node, set<string>> parse_file(meta_context& ctx, path const& file)
    {
        parser_impl impl{};
        impl.load_file(file);
        return parse_impl(ctx, impl);
    }

    tuple<xaml_node, set<string>> parse_string(meta_context& ctx, string_view xml)
    {
        parser_impl impl{};
        impl.load_string(xml);
        return parse_impl(ctx, impl);
    }
} // namespace xaml
