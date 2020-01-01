#include <filesystem>
#include <sstream>
#include <xaml/parser.hpp>

using namespace std;
using namespace std::filesystem;

namespace xaml
{
    static string get_no_default_constructor_error(type_index t)
    {
        ostringstream oss;
        oss << "Type \"" << t.name() << "\" doesn't have a default constructor.";
        return oss.str();
    }

    xaml_no_default_constructor::xaml_no_default_constructor(type_index t) : logic_error(get_no_default_constructor_error(t))
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

    static string get_xaml_no_member(type_index type, string_view name)
    {
        ostringstream oss;
        oss << "There's no member named " << name << " in type \"" << type.name() << "\".";
        return oss.str();
    }

    xaml_no_member::xaml_no_member(type_index type, string_view name) : logic_error(get_xaml_no_member(type, name))
    {
    }

    static string get_xaml_not_multicontainer(type_index type)
    {
        ostringstream oss;
        oss << "Type \"" << type.name() << "\" is not a multi-container.";
        return oss.str();
    }

    xaml_not_multicontainer::xaml_not_multicontainer(type_index type) : logic_error(get_xaml_not_multicontainer(type))
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

    parser::parser()
    {
        LIBXML_TEST_VERSION;
    }

    parser::~parser()
    {
        xmlCleanupMemory();
    }

    void parser::open(string_view file)
    {
        reader = xmlNewTextReaderFilename(absolute(file).string().c_str());
    }

    int parser::parse_members(xaml_node& mc)
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
                    if (attr_ns != "xmlns" && attr_name != "xmlns")
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
                                mc.properties.push_back({ prop, (string)attr_value });
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
                xmlTextReaderMoveToElement(reader);
                break;
            }
            case XML_TEXT_NODE:
            case XML_CDATA_SECTION_NODE:
                // TODO: default property
                break;
            case XML_ELEMENT_DECL:
            {
                string_view ns = get_string_view(xmlTextReaderConstNamespaceUri(reader));
                string_view name = get_string_view(xmlTextReaderConstName(reader));
                auto t = *get_type(ns, name);
                if (mc.type != t)
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
                        auto [ret, child] = deserialize_impl();
                        if (ret != 1)
                        {
                            clean_up(ret);
                        }
                        else if (mc.type == *t)
                        {
                            auto prop = get_property(mc.type, prop_name);
                            if (prop.can_write())
                            {
                                mc.construct_properties.push_back({ prop, move(child) });
                            }
                        }
                        else
                        {
                            auto prop = get_attach_property(*t, prop_name);
                            if (prop.can_write())
                            {
                                mc.construct_properties.push_back({ prop, move(child) });
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
                    auto [ret, child] = deserialize_impl();
                    if (ret != 1)
                    {
                        clean_up(ret);
                    }
                    else
                    {
                        mc.children.push_back(move(child));
                        ret = xmlTextReaderRead(reader);
                    }
                }
            }
        }
        return ret;
    }

    void parser::clean_up(int ret)
    {
        xmlFreeTextReader(reader);
        reader = nullptr;
        if (ret < 0)
        {
            throw xaml_parse_error();
        }
    }

    tuple<int, xaml_node> parser::deserialize_impl()
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

    xaml_node parser::parse()
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
