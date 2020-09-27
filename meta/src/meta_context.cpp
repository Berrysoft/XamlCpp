#include <xaml/meta/meta_context.h>
#include <xaml/observable_vector.h>

using namespace std;

struct xaml_meta_context_impl : xaml_implement<xaml_meta_context_impl, xaml_meta_context>
{
private:
    xaml_ptr<xaml_map<xaml_string, xaml_module>> m_modules;
    xaml_ptr<xaml_map<xaml_string, xaml_string>> m_namespace;
    xaml_ptr<xaml_map<xaml_guid, xaml_reflection_info>> m_type_info_map;
    xaml_ptr<xaml_map<xaml_guid, xaml_string>> m_basic_type_info_map;
    xaml_ptr<xaml_map<xaml_string, xaml_reflection_info>> m_name_info_map;

public:
    xaml_result init() noexcept
    {
        XAML_RETURN_IF_FAILED(xaml_map_new(&m_type_info_map));
        XAML_RETURN_IF_FAILED(xaml_map_new(&m_basic_type_info_map));
        xaml_ptr<xaml_hasher<xaml_string>> string_hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&string_hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new(string_hasher.get(), &m_modules));
        XAML_RETURN_IF_FAILED(xaml_map_new(string_hasher.get(), &m_namespace));
        XAML_RETURN_IF_FAILED(xaml_map_new(string_hasher.get(), &m_name_info_map));

#define AT(type)                                                                                           \
    do                                                                                                     \
    {                                                                                                      \
        xaml_ptr<xaml_string> __name;                                                                      \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#type), &__name));                                         \
        xaml_ptr<xaml_basic_type_info> __info;                                                             \
        XAML_RETURN_IF_FAILED(xaml_basic_type_info_new(xaml_type_guid_v<type>, __name, nullptr, &__info)); \
        XAML_RETURN_IF_FAILED(add_type(__info));                                                           \
    } while (0)

        AT(xaml_object);
        AT(xaml_string);
        //AT(xaml_vector_view);
        //AT(xaml_vector);
        //AT(xaml_map_view);
        //AT(xaml_map);
        //AT(xaml_observable_vector);
        AT(xaml_converter);
        AT(bool);
        AT(char);
        AT(int8_t);
        AT(int16_t);
        AT(int32_t);
        AT(int64_t);
        AT(uint8_t);
        AT(uint16_t);
        AT(uint32_t);
        AT(uint64_t);
        AT(float);
        AT(double);
        AT(xaml_guid);

#undef AT
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_modules(xaml_map_view<xaml_string, xaml_module>** ptr) noexcept override
    {
        return m_modules->query(ptr);
    }

    xaml_result XAML_CALL add_module(xaml_module* mod) noexcept override
    {
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(mod->get_name(&name));
        bool contains;
        XAML_RETURN_IF_FAILED(m_modules->has_key(name, &contains));
        if (!contains)
        {
            xaml_ptr<xaml_module_info> info;
            XAML_RETURN_IF_FAILED(mod->get_info(&info));
            XAML_RETURN_IF_FAILED(info->register_types(this));
            bool replaced;
            XAML_RETURN_IF_FAILED(m_modules->insert(name, mod, &replaced));
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL add_module_recursive(xaml_module* mod) noexcept override
    {
        XAML_RETURN_IF_FAILED(add_module(mod));
        xaml_ptr<xaml_module_info> info;
        XAML_RETURN_IF_FAILED(mod->get_info(&info));
        xaml_ptr<xaml_vector_view<xaml_string>> dependencies;
        XAML_RETURN_IF_FAILED(info->get_dependencies(&dependencies));
        if (dependencies)
        {
            XAML_FOREACH_START(xaml_string, dep, dependencies);
            {
                xaml_ptr<xaml_module> m;
                XAML_RETURN_IF_FAILED(xaml_module_new(&m));
                XAML_RETURN_IF_FAILED(m->open(dep));
                XAML_RETURN_IF_FAILED(add_module_recursive(m));
            }
            XAML_FOREACH_END();
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_namespace(xaml_string* xml_ns, xaml_string** ptr) noexcept override
    {
        return m_namespace->lookup(xml_ns, ptr);
    }

    xaml_result XAML_CALL add_namespace(xaml_string* xml_ns, xaml_string* ns) noexcept override
    {
        return m_namespace->insert(xml_ns, ns, nullptr);
    }

    xaml_result XAML_CALL get_types(xaml_map_view<xaml_guid, xaml_reflection_info>** ptr) noexcept override
    {
        return m_type_info_map->query(ptr);
    }

    xaml_result XAML_CALL get_type(xaml_guid const& type, xaml_reflection_info** ptr) noexcept override
    {
        return m_type_info_map->lookup(type, ptr);
    }

    xaml_result XAML_CALL get_type_by_name(xaml_string* name, xaml_reflection_info** ptr) noexcept override
    {
        return m_name_info_map->lookup(name, ptr);
    }

    xaml_result XAML_CALL get_type_by_namespace_name(xaml_string* ns, xaml_string* name, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_string> real_name;
        XAML_RETURN_IF_FAILED(get_name_by_namespace_name(ns, name, &real_name));
        return get_type_by_name(real_name, ptr);
    }

    xaml_result XAML_CALL get_name_by_namespace_name(xaml_string* ns, xaml_string* name, xaml_string** ptr) noexcept override
    {
        xaml_ptr<xaml_string> real_ns = ns;
        {
            xaml_ptr<xaml_string> item;
            if (XAML_SUCCEEDED(m_namespace->lookup(ns, &item)))
            {
                real_ns = item;
            }
        }
        std::string_view ns_view;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(real_ns, &ns_view));
        std::string_view name_view;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(name, &name_view));
        xaml_ptr<xaml_string> real_name;
        XAML_RETURN_IF_FAILED(xaml_string_new((std::string)ns_view + U("_") + (std::string)name_view, &real_name));
        return real_name->query(ptr);
    }

    xaml_result XAML_CALL add_type(xaml_reflection_info* info) noexcept override
    {
        xaml_guid type;
        XAML_RETURN_IF_FAILED(info->get_type(&type));
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(info->get_name(&name));
        XAML_RETURN_IF_FAILED(m_type_info_map->insert(type, info, nullptr));
        return m_name_info_map->insert(name, info, nullptr);
    }

    xaml_result XAML_CALL get_basic_type(xaml_guid const& type, xaml_string** ptr) noexcept
    {
        return m_basic_type_info_map->lookup(type, ptr);
    }

    xaml_result XAML_CALL add_basic_type(xaml_guid const& type, xaml_string* name) noexcept
    {
        return m_basic_type_info_map->insert(type, name, nullptr);
    }

    static xaml_result XAML_CALL get_property_changed_event_name(xaml_ptr<xaml_string> const& name, xaml_string** ptr) noexcept
    {
        std::string name_view;
        XAML_RETURN_IF_FAILED(to_string(name, &name_view));
        name_view += U("_changed");
        return xaml_string_new(move(name_view), ptr);
    }

    xaml_result XAML_CALL bind(xaml_weak_reference* wtarget, xaml_string* target_prop, xaml_weak_reference* wsource, xaml_string* source_prop, xaml_binding_mode mode, xaml_converter* converter, xaml_object* parameter, xaml_string* language) noexcept override
    {
        xaml_ptr<xaml_object> starget;
        XAML_RETURN_IF_FAILED(wtarget->resolve(&starget));
        xaml_ptr<xaml_object> ssource;
        XAML_RETURN_IF_FAILED(wsource->resolve(&ssource));
        if (!(starget && ssource)) return XAML_E_INVALIDARG;
        xaml_ptr<xaml_type_info> target_type;
        {
            xaml_guid id;
            XAML_RETURN_IF_FAILED(starget->get_guid(&id));
            xaml_ptr<xaml_reflection_info> info;
            XAML_RETURN_IF_FAILED(get_type(id, &info));
            XAML_RETURN_IF_FAILED(info->query(&target_type));
        }
        xaml_ptr<xaml_type_info> source_type;
        {
            xaml_guid id;
            XAML_RETURN_IF_FAILED(ssource->get_guid(&id));
            xaml_ptr<xaml_reflection_info> info;
            XAML_RETURN_IF_FAILED(get_type(id, &info));
            XAML_RETURN_IF_FAILED(info->query(&source_type));
        }
        xaml_ptr<xaml_property_info> targetp;
        XAML_RETURN_IF_FAILED(target_type->get_property(target_prop, &targetp));
        xaml_ptr<xaml_property_info> sourcep;
        XAML_RETURN_IF_FAILED(source_type->get_property(source_prop, &sourcep));
        xaml_ptr<xaml_converter> conv_ptr = converter;
        xaml_ptr<xaml_object> conv_param = parameter;
        xaml_ptr<xaml_string> conv_lang = language;

        auto set_to_target =
            [wsource = xaml_ptr<xaml_weak_reference>{ wsource }, sourcep,
             wtarget = xaml_ptr<xaml_weak_reference>{ wtarget }, targetp,
             conv_ptr, conv_param, conv_lang]() noexcept -> xaml_result {
            xaml_ptr<xaml_object> source;
            XAML_RETURN_IF_FAILED(wsource->resolve(&source));
            xaml_ptr<xaml_object> target;
            XAML_RETURN_IF_FAILED(wtarget->resolve(&target));
            if (source && target)
            {
                xaml_ptr<xaml_object> value;
                XAML_RETURN_IF_FAILED(sourcep->get(source, &value));
                if (conv_ptr)
                {
                    xaml_guid type;
                    XAML_RETURN_IF_FAILED(sourcep->get_type(&type));
                    xaml_ptr<xaml_object> conv_value;
                    XAML_RETURN_IF_FAILED(conv_ptr->convert(value, type, conv_param, conv_lang, &conv_value));
                    value = conv_value;
                }
                XAML_RETURN_IF_FAILED(targetp->set(target, value));
            }
            return XAML_S_OK;
        };

        auto set_to_source =
            [wsource = xaml_ptr<xaml_weak_reference>{ wsource }, sourcep,
             wtarget = xaml_ptr<xaml_weak_reference>{ wtarget }, targetp,
             conv_ptr, conv_param, conv_lang]() noexcept -> xaml_result {
            xaml_ptr<xaml_object> source;
            XAML_RETURN_IF_FAILED(wsource->resolve(&source));
            xaml_ptr<xaml_object> target;
            XAML_RETURN_IF_FAILED(wtarget->resolve(&target));
            if (source && target)
            {
                xaml_ptr<xaml_object> value;
                XAML_RETURN_IF_FAILED(targetp->get(target, &value));
                if (conv_ptr)
                {
                    xaml_guid type;
                    XAML_RETURN_IF_FAILED(targetp->get_type(&type));
                    xaml_ptr<xaml_object> conv_value;
                    XAML_RETURN_IF_FAILED(conv_ptr->convert_back(value, type, conv_param, conv_lang, &conv_value));
                    value = conv_value;
                }
                XAML_RETURN_IF_FAILED(sourcep->set(source, value));
            }
            return XAML_S_OK;
        };

        if (mode & xaml_binding_one_way)
        {
            xaml_ptr<xaml_event_info> sourcee;
            {
                xaml_ptr<xaml_string> name;
                XAML_RETURN_IF_FAILED(get_property_changed_event_name(source_prop, &name));
                XAML_RETURN_IF_FAILED(source_type->get_event(name, &sourcee));
            }

            xaml_ptr<xaml_method_info> callback;
            XAML_RETURN_IF_FAILED(xaml_method_info_new(
                nullptr, [set_to_target](xaml_vector_view<xaml_object>*) -> xaml_result { return set_to_target(); }, nullptr, &callback));
            int32_t token;
            XAML_RETURN_IF_FAILED(sourcee->add(ssource, callback, &token));
            XAML_RETURN_IF_FAILED(set_to_target());
        }
        if (mode & xaml_binding_one_way_to_source)
        {
            xaml_ptr<xaml_event_info> targete;
            {
                xaml_ptr<xaml_string> name;
                XAML_RETURN_IF_FAILED(get_property_changed_event_name(target_prop, &name));
                XAML_RETURN_IF_FAILED(target_type->get_event(name, &targete));
            }

            xaml_ptr<xaml_method_info> callback;
            XAML_RETURN_IF_FAILED(xaml_method_info_new(
                nullptr, [set_to_source](xaml_vector_view<xaml_object>*) -> xaml_result { return set_to_source(); }, nullptr, &callback));
            int32_t token;
            XAML_RETURN_IF_FAILED(targete->add(starget, callback, &token));
            XAML_RETURN_IF_FAILED(set_to_source());
        }
        if (mode == xaml_binding_one_time)
        {
            XAML_RETURN_IF_FAILED(set_to_target());
        }
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_meta_context_new(xaml_meta_context** ptr) noexcept
{
    return xaml_object_init<xaml_meta_context_impl>(ptr);
}
