#include <sstream>
#include <xaml/parser.hpp>

using namespace std;

namespace xaml
{
    static string get_no_default_constructor_error(type_index t)
    {
        ostringstream oss;
        oss << "Type \"" << t.name() << "\" doesn't have a default constructor.";
        return oss.str();
    }

    XAML_API xaml_no_default_constructor::xaml_no_default_constructor(type_index t) : logic_error(get_no_default_constructor_error(t))
    {
    }

    static string get_bad_type_error(string_view ns, string_view name)
    {
        ostringstream oss;
        oss << "Cannot find type named \"" << name << "\" in namespace \"" << ns << "\".";
        return oss.str();
    }

    XAML_API xaml_bad_type::xaml_bad_type(string_view ns, string_view name) : logic_error(get_bad_type_error(ns, name))
    {
    }

    static string get_xaml_no_member(type_index type, string_view name)
    {
        ostringstream oss;
        oss << "There's no member named " << name << " in type \"" << type.name() << "\".";
        return oss.str();
    }

    XAML_API xaml_no_member::xaml_no_member(type_index type, string_view name) : logic_error(get_xaml_no_member(type, name))
    {
    }

    static string get_xaml_not_multicontainer(type_index type)
    {
        ostringstream oss;
        oss << "Type \"" << type.name() << "\" is not a multi-container.";
        return oss.str();
    }

    XAML_API xaml_not_multicontainer::xaml_not_multicontainer(type_index type) : logic_error(get_xaml_not_multicontainer(type))
    {
    }

    static inline string_view get_string_view(const xmlChar* str)
    {
        if (str)
        {
            return (const char*)str;
        }
        else
        {
            return {};
        }
    }

    XAML_API parser::parser()
    {
        LIBXML_TEST_VERSION;
    }

    XAML_API parser::~parser()
    {
        if (!reader)
        {
            xmlFreeTextReader(reader);
            reader = nullptr;
        }
        xmlCleanupMemory();
    }

    XAML_API void parser::open(string_view file)
    {
        reader = xmlNewTextReaderFilename(file.data());
    }

    static ostream& write_valid_name(ostream& stream, string_view name)
    {
        for (char c : name)
        {
            if (isalpha(c) || isdigit(c))
            {
                stream << c;
            }
            else
            {
                stream << '_';
            }
        }
        return stream;
    }

    static string get_random_name(type_index type)
    {
        static size_t index = 0;
        ostringstream oss;
        write_valid_name(oss << "__", type.name()) << "__" << hex << type.hash_code() << "__" << dec << index++;
        return oss.str();
    }

    XAML_API markup_node parser::parse_markup(string_view value)
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
        auto t = get_type(ns, name);
        if (t)
        {
            markup_node node{ *t, get_random_name(*t) };
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
                    auto def_attr = get_attribute<default_property>(*t);
                    if (def_attr)
                    {
                        prop_name = static_pointer_cast<default_property>(def_attr)->get_property_name();
                    }
                }
                auto prop = get_property(*t, prop_name);
                if (prop.can_write())
                {
                    node.properties.push_back({ prop, (string)prop_value });
                }
                else
                {
                    throw xaml_no_member(*t, prop_name);
                }
            }
            return node;
        }
        else
        {
            throw xaml_bad_type(ns, name);
        }
    }

    static constexpr string_view x_ns{ "https://github.com/Berrysoft/XamlCpp/xaml/" };

    XAML_API int parser::parse_members(xaml_node& mc)
    {
        int ret = 1;
        while (ret == 1)
        {
            switch (xmlTextReaderNodeType(reader))
            {
            case XML_ELEMENT_NODE:
            {
                int count = xmlTextReaderAttributeCount(reader);
                string_view ns = get_string_view(xmlTextReaderConstNamespaceUri(reader));
                for (int i = 0; i < count; i++)
                {
                    xmlTextReaderMoveToAttributeNo(reader, i);
                    string_view attr_name = get_string_view(xmlTextReaderConstName(reader));
                    string_view attr_ns = get_string_view(xmlTextReaderConstNamespaceUri(reader));
                    if (attr_ns.empty()) attr_ns = ns;
                    if (attr_ns == x_ns)
                    {
                        if (attr_name == "x:name")
                        {
                            string_view attr_value = get_string_view(xmlTextReaderConstValue(reader));
                            mc.name = attr_value;
                        }
                    }
                    else if (attr_ns != "xmlns" && attr_name != "xmlns")
                    {
                        size_t dm_index = attr_name.find_first_of('.');
                        if (dm_index != string_view::npos)
                        {
                            string_view class_name = attr_name.substr(0, dm_index);
                            string_view attach_prop_name = attr_name.substr(dm_index + 1);
                            auto t = get_type(attr_ns, class_name);
                            if (t)
                            {
                                auto prop = get_attach_property(*t, attach_prop_name);
                                if (prop.can_write())
                                {
                                    string_view attr_value = get_string_view(xmlTextReaderConstValue(reader));
                                    mc.properties.push_back({ prop, (string)attr_value });
                                }
                            }
                            else
                            {
                                throw xaml_bad_type(attr_ns, class_name);
                            }
                        }
                        else
                        {
                            auto prop = get_property(mc.type, attr_name);
                            if (prop.can_write())
                            {
                                string_view attr_value = get_string_view(xmlTextReaderConstValue(reader));
                                if (attr_value.front() == '{' && attr_value.back() == '}')
                                {
                                    auto ex = parse_markup(attr_value.substr(1, attr_value.length() - 2));
                                    mc.properties.push_back({ prop, ex });
                                }
                                else
                                {
                                    mc.properties.push_back({ prop, (string)attr_value });
                                }
                            }
                            else if (!prop.can_read())
                            {
                                auto ev = get_event(mc.type, attr_name);
                                if (ev.can_add())
                                {
                                    string_view attr_value = get_string_view(xmlTextReaderConstValue(reader));
                                    mc.events.push_back({ ev, (string)attr_value });
                                }
                            }
                            else
                            {
                                throw xaml_no_member(mc.type, attr_name);
                            }
                        }
                    }
                }
                if (mc.name.empty())
                {
                    mc.name = get_random_name(mc.type);
                }
                xmlTextReaderMoveToElement(reader);
                break;
            }
            case XML_TEXT_NODE:
            case XML_CDATA_SECTION_NODE:
            {
                auto def_attr = get_attribute<default_property>(mc.type);
                if (def_attr)
                {
                    string_view prop_name = static_pointer_cast<default_property>(def_attr)->get_property_name();
                    auto prop = get_property(mc.type, prop_name);
                    if (prop.can_write())
                    {
                        mc.properties.push_back({ prop, (string)get_string_view(xmlTextReaderConstValue(reader)) });
                    }
                    else
                    {
                        throw xaml_no_member(mc.type, prop_name);
                    }
                }
                break;
            }
            case XML_ELEMENT_DECL:
            {
                string_view ns = get_string_view(xmlTextReaderConstNamespaceUri(reader));
                string_view name = get_string_view(xmlTextReaderConstName(reader));
                auto t = *get_type(ns, name);
                if (mc.type == t)
                {
                    return ret;
                }
                break;
            }
            }
            if (xmlTextReaderIsEmptyElement(reader))
            {
                return ret;
            }
            ret = xmlTextReaderRead(reader);
            if (xmlTextReaderNodeType(reader) == XML_ELEMENT_NODE)
            {
                string_view name = get_string_view(xmlTextReaderConstName(reader));
                size_t dm_index = name.find_first_of('.');
                if (dm_index != string_view::npos)
                {
                    string_view ns = get_string_view(xmlTextReaderConstNamespaceUri(reader));
                    string_view class_name = name.substr(0, dm_index);
                    string_view prop_name = name.substr(dm_index + 1);
                    auto t = get_type(ns, class_name);
                    if (t)
                    {
                        auto [ret, child] = parse_impl();
                        if (ret != 1)
                        {
                            clean_up(ret);
                        }
                        else if (mc.type == *t)
                        {
                            auto prop = get_property(mc.type, prop_name);
                            if (prop.can_write())
                            {
                                mc.properties.push_back({ prop, move(child) });
                            }
                        }
                        else
                        {
                            auto prop = get_attach_property(*t, prop_name);
                            if (prop.can_write())
                            {
                                mc.properties.push_back({ prop, move(child) });
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
                    auto [ret, child] = parse_impl();
                    if (ret != 1)
                    {
                        clean_up(ret);
                    }
                    else
                    {
                        bool is_container = invoke_static_method<bool>(mc.type, "is_container").value_or(false);
                        bool is_multicontainer = invoke_static_method<bool>(mc.type, "is_multicontainer").value_or(false);
                        if (is_container)
                        {
                            if (is_multicontainer)
                            {
                                auto& prop = mc.collection_properties["child"];
                                prop.info = get_collection_property(mc.type, "child");
                                prop.values.push_back(move(child));
                            }
                            else
                            {
                                auto prop = get_property(mc.type, "child");
                                mc.properties.push_back({ prop, move(child) });
                            }
                        }
                        ret = xmlTextReaderRead(reader);
                    }
                }
            }
        }
        return ret;
    }

    XAML_API void parser::clean_up(int ret)
    {
        xmlFreeTextReader(reader);
        reader = nullptr;
        if (ret < 0)
        {
            throw xaml_parse_error();
        }
    }

    XAML_API tuple<int, xaml_node> parser::parse_impl()
    {
        string_view ns = get_string_view(xmlTextReaderConstNamespaceUri(reader));
        string_view name = get_string_view(xmlTextReaderConstName(reader));
        auto t = get_type(ns, name);
        if (t)
        {
            xaml_node mc{ *t };
            int ret = parse_members(mc);
            return make_tuple(ret, mc);
        }
        else
        {
            throw xaml_bad_type(ns, name);
        }
    }

    XAML_API xaml_node parser::parse()
    {
        int ret = xmlTextReaderRead(reader);
        if (ret == 1)
        {
            string_view ns = get_string_view(xmlTextReaderConstNamespaceUri(reader));
            string_view name = get_string_view(xmlTextReaderConstName(reader));
            auto t = get_type(ns, name);
            if (t)
            {
                xaml_node mc{ *t };
                ret = parse_members(mc);
                clean_up(ret);
                return mc;
            }
            else
            {
                throw xaml_bad_type(ns, name);
            }
        }
        clean_up(ret);
        return { type_index(typeid(nullptr_t)) };
    }
} // namespace xaml
