#include <filesystem>
#include <locale>
#include <sstream>
#include <stack>
#include <xaml/deserialize.hpp>
#include <xaml/ui/control.hpp>

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

    static string get_bad_type_error(type_index lhs, type_index rhs)
    {
        ostringstream oss;
        oss << "\"" << lhs.name() << "\" and \"" << rhs.name() << "\" are not the same type.";
        return oss.str();
    }

    xaml_bad_type::xaml_bad_type(type_index lhs, type_index rhs) : logic_error(get_bad_type_error(lhs, rhs))
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

#ifdef UNICODE
    static inline string_t get_conv_string(string_view str)
    {
        mbstate_t mb = mbstate_t();
        string_t internal(str.size(), U('\0'));
        auto& f = use_facet<codecvt<char_t, char, mbstate_t>>(locale());
        const char* from_next;
        char_t* to_next;
        f.in(mb, str.data(), str.data() + str.size(), from_next, internal.data(), internal.data() + internal.size(), to_next);
        internal.resize(to_next - &internal.front());
        return internal;
    }
#else
    static constexpr string_view_t get_conv_string(string_view str)
    {
        return str;
    }
#endif // UNICODE

    deserializer::deserializer()
    {
        LIBXML_TEST_VERSION;
    }

    deserializer::~deserializer()
    {
        xmlCleanupMemory();
    }

    void deserializer::open(string_view file)
    {
        reader = xmlNewTextReaderFilename(absolute(file).string().c_str());
    }

    int deserializer::deserialize_members(shared_ptr<meta_class> mc)
    {
        int ret = 1;
        while (ret == 1)
        {
            switch (xmlTextReaderNodeType(reader))
            {
            case XML_ELEMENT_NODE:
            {
                int count = xmlTextReaderAttributeCount(reader);
                for (int i = 0; i < count; i++)
                {
                    xmlTextReaderMoveToAttributeNo(reader, i);
                    string_view attr_name = get_string_view(xmlTextReaderConstName(reader));
                    auto prop = get_property(mc->this_type(), attr_name);
                    if (prop.can_write())
                    {
                        string_view attr_value = get_string_view(xmlTextReaderConstValue(reader));
                        prop.set(mc, (string_view_t)get_conv_string(attr_value));
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
                if (mc->this_type() != t)
                {
                    return ret;
                }
                break;
            }
            }
            ret = xmlTextReaderRead(reader);
            if (xmlTextReaderNodeType(reader) == XML_ELEMENT_NODE)
            {
                auto [ret, child] = deserialize_impl();
                if (!child)
                {
                    clean_up(ret);
                }
                else
                {
                    auto prop = get_property(mc->this_type(), "child");
                    if (prop.can_write())
                    {
                        prop.set(mc, dynamic_pointer_cast<control>(child));
                    }
                }
            }
        }
        return ret;
    }

    void deserializer::clean_up(int ret)
    {
        xmlFreeTextReader(reader);
        reader = nullptr;
        if (ret < 0)
        {
            throw xaml_parse_error();
        }
    }

    tuple<int, shared_ptr<meta_class>> deserializer::deserialize_impl()
    {
        string_view ns = get_string_view(xmlTextReaderConstNamespaceUri(reader));
        string_view name = get_string_view(xmlTextReaderConstName(reader));
        auto t = get_type(ns, name);
        if (t)
        {
            auto mc = construct(*t);
            int ret = 1;
            if (mc)
            {
                ret = deserialize_members(mc);
            }
            else
            {
                throw xaml_no_default_constructor(*t);
            }
            return make_tuple(ret, mc);
        }
        else
        {
            throw xaml_bad_type(ns, name);
        }
    }

    shared_ptr<meta_class> deserializer::deserialize()
    {
        int ret = xmlTextReaderRead(reader);
        shared_ptr<meta_class> result;
        if (ret == 1)
        {
            std::tie(ret, result) = deserialize_impl();
        }
        clean_up(ret);
        return result;
    }

    void deserializer::deserialize(shared_ptr<meta_class> mc)
    {
        if (mc)
        {
            int ret = xmlTextReaderRead(reader);
            if (ret == 1)
            {
                string_view ns = get_string_view(xmlTextReaderConstNamespaceUri(reader));
                string_view name = get_string_view(xmlTextReaderConstName(reader));
                auto t = get_type(ns, name);
                if (t && mc->this_type() == *t)
                {
                    ret = deserialize_members(mc);
                }
                else
                {
                    if (t)
                        throw xaml_bad_type(mc->this_type(), *t);
                    else
                        throw xaml_bad_type(ns, name);
                }
            }
            clean_up(ret);
        }
    }
} // namespace xaml
