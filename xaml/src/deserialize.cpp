#include <sstream>
#include <xaml/deserialize.hpp>

using namespace std;

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

    xaml_deserializer::xaml_deserializer()
    {
        LIBXML_TEST_VERSION;
    }

    xaml_deserializer::~xaml_deserializer()
    {
        xmlCleanupMemory();
    }

    void xaml_deserializer::open(string_view file)
    {
        reader = xmlReaderForFile(file.data(), nullptr, 0);
    }

    int xaml_deserializer::deserialize_members(shared_ptr<meta_class> mc)
    {
        int ret;
        while ((ret = xmlTextReaderRead(reader)) == 1)
        {
        }
        return ret;
    }

    void xaml_deserializer::clean_up(int ret)
    {
        xmlFreeTextReader(reader);
        reader = nullptr;
        if (ret)
        {
            throw xaml_parse_error();
        }
    }

    shared_ptr<meta_class> xaml_deserializer::deserialize()
    {
        int ret = xmlTextReaderRead(reader);
        if (ret == 1)
        {
            string_view ns = (const char*)xmlTextReaderConstNamespaceUri(reader);
            string_view name = (const char*)xmlTextReaderConstName(reader);
            auto t = get_type(ns, name);
            if (t)
            {
                auto mc = construct(*t);
                if (mc)
                {
                    ret = deserialize_members(mc);
                }
                else
                {
                    throw xaml_no_default_constructor(*t);
                }
                return mc;
            }
            else
            {
                throw xaml_bad_type(ns, name);
            }
        }
        clean_up(ret);
        return nullptr;
    }

    void xaml_deserializer::deserialize(shared_ptr<meta_class> mc)
    {
        if (mc)
        {
            int ret = xmlTextReaderRead(reader);
            if (ret == 1)
            {
                string_view ns = (const char*)xmlTextReaderConstNamespaceUri(reader);
                string_view name = (const char*)xmlTextReaderConstName(reader);
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
