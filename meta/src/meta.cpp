#include <xaml/meta/meta.hpp>

using namespace std;

namespace xaml
{
    meta_class const* reflection_info::__get_attribute(type_index attr_type) const noexcept
    {
        auto it = m_attribute_map.find(attr_type);
        if (it != m_attribute_map.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    void reflection_info::set_attribute(unique_ptr<meta_class>&& attr) noexcept
    {
        if (attr)
        {
            m_attribute_map[attr->this_type()] = move(attr);
        }
    }

    __type_erased_function const* reflection_info::__get_static_method(string_view name, type_index ret_type, initializer_list<type_index> arg_types) const noexcept
    {
        auto its = m_static_method_map.equal_range((string)name);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types) && it->second->is_same_return_type(ret_type))
            {
                return it->second.get();
            }
        }
        return nullptr;
    }

    void reflection_info::__add_static_method(string_view name, unique_ptr<__type_erased_function>&& func) noexcept
    {
        m_static_method_map.emplace((string)name, move(func));
    }

    __type_erased_function const* reflection_info::__get_constructor(initializer_list<type_index> arg_types) const noexcept
    {
        for (auto& ctor : m_ctors)
        {
            if (ctor->is_same_arg_type(arg_types))
            {
                return ctor.get();
            }
        }
        return nullptr;
    }

    void reflection_info::__add_constructor(unique_ptr<__type_erased_function>&& ctor) noexcept
    {
        m_ctors.push_back(move(ctor));
    }

    __type_erased_function const* reflection_info::__get_method(string_view name, type_index ret_type, initializer_list<type_index> arg_types) const noexcept
    {
        auto its = m_method_map.equal_range((string)name);
        for (auto it = its.first; it != its.second; ++it)
        {
            if (it->second->is_same_arg_type(arg_types) && it->second->is_same_return_type(ret_type))
            {
                return it->second.get();
            }
        }
        return nullptr;
    }

    void reflection_info::__add_method(string_view name, unique_ptr<__type_erased_function>&& func) noexcept
    {
        m_method_map.emplace((string)name, move(func));
    }

    property_info const* reflection_info::get_property(string_view name) const noexcept
    {
        auto it = m_prop_map.find((string)name);
        if (it != m_prop_map.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    void reflection_info::__add_property(string_view name, type_index type, function<any(meta_class*)>&& getter, function<void(meta_class*, any)>&& setter, bool attach)
    {
        auto prop = make_unique<property_info>();
        prop->m_name = name;
        prop->m_type = type;
        prop->getter = move(getter);
        prop->setter = move(setter);
        m_prop_map[prop->m_name] = move(prop);
    }

    collection_property_info const* reflection_info::get_collection_property(string_view name) const noexcept
    {
        auto it = m_collection_prop_map.find((string)name);
        if (it != m_collection_prop_map.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    void reflection_info::__add_collection_property(string_view name, type_index type, function<void(meta_class*, any)>&& adder, function<void(meta_class*, any)>&& remover, bool attach)
    {
        auto prop = make_unique<collection_property_info>();
        prop->m_name = name;
        prop->m_type = type;
        prop->adder = move(adder);
        prop->remover = move(remover);
        m_collection_prop_map[prop->m_name] = move(prop);
    }

    event_info const* reflection_info::get_event(string_view name) const noexcept
    {
        auto it = m_event_map.find((string)name);
        if (it != m_event_map.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    void reflection_info::__add_event(string_view name, function<size_t(meta_class*, __type_erased_function const*)>&& adder, std::function<std::size_t(meta_class*, meta_class*, __type_erased_function const*)>&& adder_erased_this, function<void(meta_class*, size_t)>&& remover, unique_ptr<__type_erased_function>&& invoker)
    {
        auto ev = make_unique<event_info>();
        ev->m_name = name;
        ev->adder = move(adder);
        ev->adder_erased_this = move(adder_erased_this);
        ev->remover = move(remover);
        ev->invoker = move(invoker);
        m_event_map[ev->m_name] = move(ev);
    }

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

    reflection_info const* meta_context::get_reflection(type_index type) const noexcept
    {
        auto it = ref_map.find(type);
        if (it != ref_map.end())
        {
            return it->second.get();
        }
        return nullptr;
    }

    void meta_context::register_type(unique_ptr<reflection_info>&& ref) noexcept
    {
        auto type = ref->get_type();
        auto [ns, name] = ref->get_type_name();
        type_map[(string)ns].emplace(name, type);
        ref_map[type] = move(ref);
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

    static inline string __get_property_changed_event_name(string_view name)
    {
        return (string)name + "_changed";
    }

    __binding_guard::__binding_guard(meta_context& ctx, weak_ptr<meta_class> target, string_view target_prop, weak_ptr<meta_class> source, string_view source_prop, binding_mode mode)
        : target(target), source(source)
    {
        auto starget = target.lock();
        auto ssource = source.lock();
        auto tf = ctx.get_reflection(starget->this_type());
        auto sf = ctx.get_reflection(ssource->this_type());
        this->target_prop = tf->get_property(target_prop);
        target_event = tf->get_event(__get_property_changed_event_name(target_prop));
        this->source_prop = sf->get_property(source_prop);
        source_event = sf->get_event(__get_property_changed_event_name(source_prop));
        if (mode & binding_mode::one_way)
        {
            this->target_prop->set(starget.get(), this->source_prop->get(ssource.get()));
            source_token = source_event->add(
                ssource.get(),
                function<void()>(
                    [this]() -> void {
                        if (auto ssource = this->source.lock())
                            if (auto starget = this->target.lock())
                                this->target_prop->set(starget.get(), this->source_prop->get(ssource.get()));
                    }));
        }
        if (mode & binding_mode::one_way_to_source)
        {
            this->source_prop->set(ssource.get(), this->target_prop->get(starget.get()));
            target_token = target_event->add(
                starget.get(),
                function<void()>(
                    [this]() -> void {
                        if (auto ssource = this->source.lock())
                            if (auto starget = this->target.lock())
                                this->source_prop->set(ssource.get(), this->target_prop->get(starget.get()));
                    }));
        }
    }

    __binding_guard::~__binding_guard()
    {
        if (auto ssource = source.lock())
            source_event->remove(ssource.get(), source_token);
        if (auto starget = target.lock())
            target_event->remove(starget.get(), target_token);
    }
} // namespace xaml
