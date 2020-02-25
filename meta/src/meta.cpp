#include <unordered_map>
#include <unordered_set>
#include <xaml/meta/meta.hpp>

using namespace std;

namespace xaml
{
    struct type_index_wrapper
    {
        type_index type{ typeid(nullptr_t) };
    };

    struct meta_context
    {
        unordered_map<string, string> namespace_map;
        unordered_map<string, unordered_map<string, type_index>> type_map;
        unordered_map<type_index, tuple<string, string>> type_name_map;
        unordered_set<type_index> enum_type_set;
        unordered_map<type_index, unordered_map<type_index, unique_ptr<meta_class>>> attribute_map;
        unordered_map<type_index, unordered_multimap<string, unique_ptr<__type_erased_function>>> static_method_map;
        unordered_multimap<type_index, unique_ptr<__type_erased_function>> ctor_map;
        unordered_map<type_index, unordered_multimap<string, unique_ptr<__type_erased_function>>> method_map;
        unordered_map<type_index, unordered_map<string, type_index_wrapper>> prop_type_map;
    };

    static shared_ptr<meta_context> m_current{ nullptr };

    void init_context(shared_ptr<meta_context> const& ctx)
    {
        if (ctx && ctx != m_current)
            m_current = ctx;
        else if (!m_current)
            m_current = make_shared<meta_context>();
    }

    shared_ptr<meta_context> __get_context() noexcept
    {
        return m_current;
    }

    static string get_real_namespace(string_view ns)
    {
        string sns{ ns };
        auto it = m_current->namespace_map.find(sns);
        if (it != m_current->namespace_map.end())
        {
            return it->second;
        }
        else
        {
            return sns;
        }
    }

    optional<type_index> get_type(string_view ns, string_view name) noexcept
    {
        string sns{ get_real_namespace(ns) };
        auto it = m_current->type_map[sns].find((string)name);
        if (it != m_current->type_map[sns].end())
        {
            return make_optional(it->second);
        }
        else
        {
            return nullopt;
        }
    }

    optional<tuple<string, string>> get_type_name(type_index type) noexcept
    {
        auto it = m_current->type_name_map.find(type);
        if (it != m_current->type_name_map.end())
        {
            return it->second;
        }
        else
        {
            return nullopt;
        }
    }

    void __register_type(string_view ns, string_view name, type_index type) noexcept
    {
        m_current->type_map[get_real_namespace(ns)].emplace((string)name, type);
        m_current->type_name_map[type] = make_tuple<string, string>((string)ns, (string)name);
    }

    void __register_enum(type_index type) noexcept
    {
        m_current->enum_type_set.emplace(move(type));
    }

    bool is_registered_enum(type_index type) noexcept
    {
        return m_current->enum_type_set.find(type) != m_current->enum_type_set.end();
    }

    void add_xml_namespace(string_view xmlns, string_view ns) noexcept
    {
        m_current->namespace_map.emplace((string)xmlns, (string)ns);
    }

    meta_class* __get_attribute(type_index type, type_index attr_type) noexcept
    {
        return m_current->attribute_map[type][attr_type].get();
    }

    void set_attribute(type_index type, unique_ptr<meta_class>&& attr) noexcept
    {
        if (attr)
        {
            m_current->attribute_map[type][attr->this_type()] = move(attr);
        }
    }

    __type_erased_function* __get_static_method(type_index type, string_view name, type_index ret_type, initializer_list<type_index> arg_types) noexcept
    {
        auto its = m_current->static_method_map[type].equal_range((string)name);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types) && it->second->is_same_return_type(ret_type))
            {
                return it->second.get();
            }
        }
        return nullptr;
    }

    void __add_static_method(type_index type, string_view name, unique_ptr<__type_erased_function>&& func) noexcept
    {
        m_current->static_method_map[type].emplace((string)name, move(func));
    }

    __type_erased_function* __get_constructor(type_index type, initializer_list<type_index> arg_types) noexcept
    {
        auto its = m_current->ctor_map.equal_range(type);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types))
            {
                return it->second.get();
            }
        }
        return nullptr;
    }

    void __add_constructor(type_index type, unique_ptr<__type_erased_function>&& ctor) noexcept
    {
        m_current->ctor_map.emplace(type, move(ctor));
    }

    __type_erased_function* __get_method(type_index type, string_view name, type_index ret_type, initializer_list<type_index> arg_types) noexcept
    {
        auto its = m_current->method_map[type].equal_range((string)name);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types) && it->second->is_same_return_type(ret_type))
            {
                return it->second.get();
            }
        }
        return nullptr;
    }

    __type_erased_function* __get_first_method(type_index type, string_view name) noexcept
    {
        auto its = m_current->method_map[type].equal_range((string)name);
        if (its.first != its.second)
        {
            return its.first->second.get();
        }
        return nullptr;
    }

    void __add_method(type_index type, string_view name, unique_ptr<__type_erased_function>&& func) noexcept
    {
        m_current->method_map[type].emplace((string)name, move(func));
    }

    type_index __get_property_type(type_index type, string_view name) noexcept
    {
        return m_current->prop_type_map[type][(string)name].type;
    }

    void __set_property_type(type_index type, string_view name, type_index prop_type) noexcept
    {
        m_current->prop_type_map[type][(string)name].type = prop_type;
    }
} // namespace xaml
