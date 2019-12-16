#include <unordered_map>
#include <xaml/meta/meta.hpp>

using namespace std;

namespace xaml
{
    static unordered_map<string, type_index> type_map;

    optional<type_index> get_type_index(string_view name) noexcept
    {
        auto it = type_map.find((string)name);
        if (it != type_map.end())
        {
            return make_optional(it->second);
        }
        else
        {
            return nullopt;
        }
    }

    void __register_type(string_view name, type_index type) noexcept
    {
        type_map.emplace((string)name, type);
    }

    static unordered_multimap<type_index, shared_ptr<details::type_erased_function>> ctor_map;

    shared_ptr<details::type_erased_function> __get_constructor(type_index type, initializer_list<type_index> arg_types) noexcept
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

    void __add_constructor(type_index type, shared_ptr<details::type_erased_function> ctor) noexcept
    {
        ctor_map.emplace(type, ctor);
    }

    static unordered_map<type_index, unordered_multimap<string, shared_ptr<details::type_erased_function>>> method_map;

    shared_ptr<details::type_erased_function> __get_method(type_index type, string_view name, type_index ret_type, initializer_list<type_index> arg_types) noexcept
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

    void __add_method(type_index type, string_view name, shared_ptr<details::type_erased_function> func) noexcept
    {
        method_map[type].emplace((string)name, func);
    }
} // namespace xaml
