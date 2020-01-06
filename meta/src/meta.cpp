#include <unordered_map>
#include <xaml/meta/meta.hpp>

using namespace std;

namespace xaml
{
    static unordered_map<string, string> namespace_map;
    static unordered_map<string, unordered_map<string, type_index>> type_map;
    static unordered_map<type_index, tuple<string, string>> type_name_map;

    static string get_real_namespace(string_view ns)
    {
        string sns{ ns };
        auto it = namespace_map.find(sns);
        if (it != namespace_map.end())
        {
            return it->second;
        }
        else
        {
            return sns;
        }
    }

    XAML_API optional<type_index> get_type(string_view ns, string_view name) noexcept
    {
        string sns{ get_real_namespace(ns) };
        auto it = type_map[sns].find((string)name);
        if (it != type_map[sns].end())
        {
            return make_optional(it->second);
        }
        else
        {
            return nullopt;
        }
    }

    XAML_API optional<tuple<string, string>> get_type_name(type_index type) noexcept
    {
        auto it = type_name_map.find(type);
        if (it != type_name_map.end())
        {
            return it->second;
        }
        else
        {
            return nullopt;
        }
    }

    XAML_API void __register_type(string_view ns, string_view name, type_index type) noexcept
    {
        type_map[get_real_namespace(ns)].emplace((string)name, type);
        type_name_map[type] = make_tuple<string, string>((string)ns, (string)name);
    }

    XAML_API void add_xml_namespace(string_view xmlns, string_view ns) noexcept
    {
        namespace_map.emplace((string)xmlns, (string)ns);
    }

    static unordered_map<type_index, unordered_map<type_index, shared_ptr<meta_class>>> attribute_map;

    XAML_API shared_ptr<meta_class> get_attribute(type_index type, type_index attr_type) noexcept
    {
        return attribute_map[type][attr_type];
    }

    XAML_API void set_attribute(type_index type, shared_ptr<meta_class> attr) noexcept
    {
        if (attr)
        {
            attribute_map[type][attr->this_type()] = attr;
        }
    }

    static unordered_map<type_index, unordered_multimap<string, shared_ptr<__type_erased_function>>> static_method_map;

    XAML_API shared_ptr<__type_erased_function> __get_static_method(type_index type, string_view name, type_index ret_type, initializer_list<type_index> arg_types) noexcept
    {
        auto its = static_method_map[type].equal_range((string)name);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types) && it->second->is_same_return_type(ret_type))
            {
                return it->second;
            }
        }
        return nullptr;
    }

    XAML_API void __add_static_method(type_index type, string_view name, shared_ptr<__type_erased_function> func) noexcept
    {
        static_method_map[type].emplace((string)name, func);
    }

    static unordered_multimap<type_index, shared_ptr<__type_erased_function>> ctor_map;

    XAML_API shared_ptr<__type_erased_function> __get_constructor(type_index type, initializer_list<type_index> arg_types) noexcept
    {
        auto its = ctor_map.equal_range(type);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types))
            {
                return it->second;
            }
        }
        return nullptr;
    }

    XAML_API void __add_constructor(type_index type, shared_ptr<__type_erased_function> ctor) noexcept
    {
        ctor_map.emplace(type, ctor);
    }

    static unordered_map<type_index, unordered_multimap<string, shared_ptr<__type_erased_function>>> method_map;

    XAML_API shared_ptr<__type_erased_function> __get_method(type_index type, string_view name, type_index ret_type, initializer_list<type_index> arg_types) noexcept
    {
        auto its = method_map[type].equal_range((string)name);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types) && it->second->is_same_return_type(ret_type))
            {
                return it->second;
            }
        }
        return nullptr;
    }

    XAML_API shared_ptr<__type_erased_function> __get_first_method(type_index type, string_view name) noexcept
    {
        auto its = method_map[type].equal_range((string)name);
        for (auto it = its.first; it != its.second; ++it)
        {
            return it->second;
        }
        return nullptr;
    }

    XAML_API void __add_method(type_index type, string_view name, shared_ptr<__type_erased_function> func) noexcept
    {
        method_map[type].emplace((string)name, func);
    }
} // namespace xaml
