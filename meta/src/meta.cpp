#include <xaml/meta/meta.hpp>

using namespace std;

namespace xaml
{
    module* meta_context::add_module(string_view path)
    {
        auto m = make_unique<module>(path);
        auto pinit = (void (*)(void*) noexcept)m->get_method("init_meta");
        if (pinit)
        {
            pinit(this);
            return modules_map.emplace(path, move(m)).first->second.get();
        }
        return nullptr;
    }

    string meta_context::get_real_namespace(string_view ns)
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

    optional<type_index> meta_context::get_type(string_view ns, string_view name) noexcept
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

    optional<tuple<string, string>> meta_context::get_type_name(type_index type) noexcept
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

    void meta_context::__register_type(string_view ns, string_view name, type_index type) noexcept
    {
        type_map[get_real_namespace(ns)].emplace((string)name, type);
        type_name_map[type] = make_tuple<string, string>((string)ns, (string)name);
    }

    void meta_context::__register_enum(type_index type) noexcept
    {
        enum_type_set.emplace(move(type));
    }

    bool meta_context::is_registered_enum(type_index type) noexcept
    {
        return enum_type_set.find(type) != enum_type_set.end();
    }

    void meta_context::add_xml_namespace(string_view xmlns, string_view ns) noexcept
    {
        namespace_map.emplace((string)xmlns, (string)ns);
    }

    meta_class* meta_context::__get_attribute(type_index type, type_index attr_type) noexcept
    {
        return attribute_map[type][attr_type].get();
    }

    void meta_context::set_attribute(type_index type, unique_ptr<meta_class>&& attr) noexcept
    {
        if (attr)
        {
            attribute_map[type][attr->this_type()] = move(attr);
        }
    }

    __type_erased_function* meta_context::__get_static_method(type_index type, string_view name, type_index ret_type, initializer_list<type_index> arg_types) noexcept
    {
        auto its = static_method_map[type].equal_range((string)name);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types) && it->second->is_same_return_type(ret_type))
            {
                return it->second.get();
            }
        }
        return nullptr;
    }

    void meta_context::__add_static_method(type_index type, string_view name, unique_ptr<__type_erased_function>&& func) noexcept
    {
        static_method_map[type].emplace((string)name, move(func));
    }

    __type_erased_function* meta_context::__get_constructor(type_index type, initializer_list<type_index> arg_types) noexcept
    {
        auto its = ctor_map.equal_range(type);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types))
            {
                return it->second.get();
            }
        }
        return nullptr;
    }

    void meta_context::__add_constructor(type_index type, unique_ptr<__type_erased_function>&& ctor) noexcept
    {
        ctor_map.emplace(type, move(ctor));
    }

    __type_erased_function* meta_context::__get_method(type_index type, string_view name, type_index ret_type, initializer_list<type_index> arg_types) noexcept
    {
        auto its = method_map[type].equal_range((string)name);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types) && it->second->is_same_return_type(ret_type))
            {
                return it->second.get();
            }
        }
        return nullptr;
    }

    __type_erased_function* meta_context::__get_first_method(type_index type, string_view name) noexcept
    {
        auto its = method_map[type].equal_range((string)name);
        if (its.first != its.second)
        {
            return its.first->second.get();
        }
        return nullptr;
    }

    void meta_context::__add_method(type_index type, string_view name, unique_ptr<__type_erased_function>&& func) noexcept
    {
        method_map[type].emplace((string)name, move(func));
    }

    type_index meta_context::__get_property_type(type_index type, string_view name) noexcept
    {
        return prop_type_map[type][(string)name].type;
    }

    void meta_context::__set_property_type(type_index type, string_view name, type_index prop_type) noexcept
    {
        prop_type_map[type][(string)name].type = prop_type;
    }

    size_t meta_context::bind(weak_ptr<meta_class> target, string_view target_prop, weak_ptr<meta_class> source, string_view source_prop, binding_mode mode)
    {
        bind_index++;
        bind_map.emplace(bind_index, make_unique<__binding_guard>(*this, target, target_prop, source, source_prop, mode));
        return bind_index;
    }

    void meta_context::unbind(size_t token)
    {
        bind_map.erase(token);
    }
} // namespace xaml
