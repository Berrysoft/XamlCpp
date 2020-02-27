#include <xaml/meta/meta.hpp>

using namespace std;

namespace xaml
{
    module* meta_context::add_module(string_view path)
    {
        using init_meta_t = void (*)(void*) noexcept;
        auto m = make_unique<module>(path);
        auto pinit = (init_meta_t)m->get_method("init_meta");
        if (pinit)
        {
            pinit(this);
            module* result = m.get();
            modules_map.emplace(path, move(m));
            return result;
        }
        return nullptr;
    }

    string meta_context::get_real_namespace(string_view ns) const
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

    optional<type_index> meta_context::get_type(string_view ns, string_view name) const noexcept
    {
        auto it = type_map.find((string)get_real_namespace(ns));
        if (it != type_map.end())
        {
            auto& map = it->second;
            auto mit = map.find((string)name);
            if (mit != map.end())
            {
                return make_optional(mit->second);
            }
        }
        return nullopt;
    }

    optional<tuple<string, string>> meta_context::get_type_name(type_index type) const noexcept
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

    bool meta_context::is_registered_enum(type_index type) const noexcept
    {
        return enum_type_set.find(type) != enum_type_set.end();
    }

    void meta_context::add_xml_namespace(string_view xmlns, string_view ns) noexcept
    {
        namespace_map.emplace((string)xmlns, (string)ns);
    }

    meta_class const* meta_context::__get_attribute(type_index type, type_index attr_type) const noexcept
    {
        auto it = attribute_map.find(type);
        if (it != attribute_map.end())
        {
            auto& map = it->second;
            auto mit = map.find(attr_type);
            if (mit != map.end())
            {
                return mit->second.get();
            }
        }
        return nullptr;
    }

    void meta_context::set_attribute(type_index type, unique_ptr<meta_class>&& attr) noexcept
    {
        if (attr)
        {
            attribute_map[type][attr->this_type()] = move(attr);
        }
    }

    __type_erased_function const* meta_context::__get_static_method(type_index type, string_view name, type_index ret_type, initializer_list<type_index> arg_types) const noexcept
    {
        auto it = static_method_map.find(type);
        if (it != static_method_map.end())
        {
            auto its = it->second.equal_range((string)name);
            for (auto it = its.first; it != its.second; ++it)
            {
                if (it->second->is_same_arg_type(arg_types) && it->second->is_same_return_type(ret_type))
                {
                    return it->second.get();
                }
            }
        }
        return nullptr;
    }

    void meta_context::__add_static_method(type_index type, string_view name, unique_ptr<__type_erased_function>&& func) noexcept
    {
        static_method_map[type].emplace((string)name, move(func));
    }

    __type_erased_function const* meta_context::__get_constructor(type_index type, initializer_list<type_index> arg_types) const noexcept
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

    __type_erased_function const* meta_context::__get_method(type_index type, string_view name, type_index ret_type, initializer_list<type_index> arg_types) const noexcept
    {
        auto it = method_map.find(type);
        if (it != method_map.end())
        {
            auto its = it->second.equal_range((string)name);
            for (auto it = its.first; it != its.second; ++it)
            {
                if (it->second->is_same_arg_type(arg_types) && it->second->is_same_return_type(ret_type))
                {
                    return it->second.get();
                }
            }
        }
        return nullptr;
    }

    __type_erased_function const* meta_context::__get_first_method(type_index type, string_view name) const noexcept
    {
        auto it = method_map.find(type);
        if (it != method_map.end())
        {
            auto its = it->second.equal_range((string)name);
            if (its.first != its.second)
            {
                return its.first->second.get();
            }
        }
        return nullptr;
    }

    void meta_context::__add_method(type_index type, string_view name, unique_ptr<__type_erased_function>&& func) noexcept
    {
        method_map[type].emplace((string)name, move(func));
    }

    type_index meta_context::__get_property_type(type_index type, string_view name) const noexcept
    {
        auto it = prop_type_map.find(type);
        if (it != prop_type_map.end())
        {
            auto& map = it->second;
            auto mit = map.find((string)name);
            if (mit != map.end())
            {
                return mit->second.type;
            }
        }
        return type_index(typeid(nullptr_t));
    }

    property_info meta_context::get_property(std::type_index type, std::string_view name) const
    {
        property_info info = {};
        info.m_name = name;
        info.m_type = __get_property_type(type, name);
        std::string pname = __property_name(name);
        info.getter = get_method<std::any>(type, pname);
        info.setter = get_method<void, std::any>(type, pname);
        return info;
    }

    property_info meta_context::get_attach_property(std::type_index type, std::string_view name) const
    {
        property_info info = {};
        info.m_name = name;
        info.m_type = __get_property_type(type, name);
        std::string pname = __attach_property_name(name);
        info.getter = get_static_method<std::any, meta_class*>(type, pname);
        info.setter = get_static_method<void, meta_class*, std::any>(type, pname);
        return info;
    }

    void meta_context::__set_property_type(type_index type, string_view name, type_index prop_type) noexcept
    {
        prop_type_map[type][(string)name].type = prop_type;
    }

    collection_property_info meta_context::get_collection_property(std::type_index type, std::string_view name) const
    {
        collection_property_info info = {};
        info.m_name = name;
        info.m_type = __get_property_type(type, name);
        info.adder = get_method<void, std::any>(type, __add_collection_property_name(name));
        info.remover = get_method<void, std::any>(type, __remove_collection_property_name(name));
        return info;
    }

    collection_property_info meta_context::get_attach_collection_property(std::type_index type, std::string_view name) const
    {
        collection_property_info info = {};
        info.m_name = name;
        info.m_type = __get_property_type(type, name);
        info.adder = get_static_method<void, meta_class*, std::any>(type, __add_attach_collection_property_name(name));
        info.remover = get_static_method<void, meta_class*, std::any>(type, __remove_attach_collection_property_name(name));
        return info;
    }

    event_info meta_context::__get_event(std::type_index type, std::string_view name, std::initializer_list<std::type_index> arg_types) const
    {
        event_info info = {};
        info.m_name = name;
        std::string ename = __event_name(name);
        info.adder = get_method<typename event_info::token_type, __type_erased_function const*>(type, ename);
        info.adder_erased_this = get_method<typename event_info::token_type, meta_class*, __type_erased_function const*>(type, ename);
        info.remover = get_method<void, typename event_info::token_type>(type, ename);
        info.invoker = __get_method(type, ename, std::type_index(typeid(void)), arg_types);
        return info;
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

    static inline std::string __get_property_changed_event_name(std::string_view name)
    {
        return (std::string)name + "_changed";
    }

    __binding_guard::__binding_guard(meta_context& ctx, weak_ptr<meta_class> target, string_view target_prop, weak_ptr<meta_class> source, string_view source_prop, binding_mode mode)
        : target(target), source(source)
    {
        auto starget = target.lock();
        auto ssource = source.lock();
        this->target_prop = ctx.get_property(target.lock()->this_type(), target_prop);
        target_event = ctx.get_event(target.lock()->this_type(), __get_property_changed_event_name(target_prop));
        this->source_prop = ctx.get_property(source.lock()->this_type(), source_prop);
        source_event = ctx.get_event(source.lock()->this_type(), __get_property_changed_event_name(source_prop));
        this->target_prop.set(starget.get(), this->source_prop.get(ssource.get()));
        if (mode & binding_mode::one_way)
        {
            source_token = source_event.add(
                ssource.get(),
                function<void()>(
                    [this]() -> void {
                        if (auto ssource = this->source.lock())
                            if (auto starget = this->target.lock())
                                this->target_prop.set(starget.get(), this->source_prop.get(ssource.get()));
                    }));
        }
        if (mode & binding_mode::one_way_to_source)
        {
            target_token = target_event.add(
                starget.get(),
                function<void()>(
                    [this]() -> void {
                        if (auto ssource = this->source.lock())
                            if (auto starget = this->target.lock())
                                this->source_prop.set(ssource.get(), this->target_prop.get(starget.get()));
                    }));
        }
    }

    __binding_guard::~__binding_guard()
    {
        if (auto ssource = source.lock())
            source_event.remove(ssource.get(), source_token);
        if (auto starget = target.lock())
            target_event.remove(starget.get(), target_token);
    }
} // namespace xaml
