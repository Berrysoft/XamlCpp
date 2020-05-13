#include <xaml/meta/meta_context.h>

struct xaml_meta_context_impl : xaml_implement<xaml_meta_context_impl, xaml_meta_context, xaml_object>
{
private:
    xaml_ptr<xaml_map> m_modules;
    xaml_ptr<xaml_map> m_namespace;
    xaml_ptr<xaml_map> m_type_info_map;
    xaml_ptr<xaml_map> m_basic_type_info_map;
    xaml_ptr<xaml_map> m_name_info_map;

public:
    xaml_result init() noexcept
    {
        xaml_ptr<xaml_hasher> guid_hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_new<xaml_guid>(&guid_hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(guid_hasher.get(), &m_type_info_map));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(guid_hasher.get(), &m_basic_type_info_map));
        xaml_ptr<xaml_hasher> string_hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&string_hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_modules));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_namespace));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(string_hasher.get(), &m_name_info_map));

#define AT(type)                                                                      \
    do                                                                                \
    {                                                                                 \
        xaml_ptr<xaml_string> __name;                                                 \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#type), &__name));                    \
        XAML_RETURN_IF_FAILED(xaml_meta_context::add_basic_type<type>(__name.get())); \
    } while (0)

        AT(xaml_object);
        AT(xaml_string);
        AT(bool);
        AT(xaml_char_t);
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

    xaml_result XAML_CALL get_modules(xaml_vector_view** ptr) noexcept override
    {
        return m_modules->query(ptr);
    }

    xaml_result XAML_CALL add_module(xaml_module* mod) noexcept override
    {
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(mod->get_name(&name));
        bool contains;
        XAML_RETURN_IF_FAILED(m_modules->has_key(name.get(), &contains));
        if (!contains)
        {
            xaml_result (*pregister)(xaml_meta_context*) noexcept;
            XAML_RETURN_IF_FAILED(mod->get_method("xaml_module_register", (void**)&pregister));
            XAML_RETURN_IF_FAILED(pregister(this));
            bool replaced;
            XAML_RETURN_IF_FAILED(m_modules->insert(name.get(), mod, &replaced));
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL add_module_recursive(xaml_module* mod) noexcept override
    {
        XAML_RETURN_IF_FAILED(add_module(mod));
        xaml_result (*pdeps)(xaml_char_t const* const**) noexcept;
        if (XAML_SUCCEEDED(mod->get_method("xaml_module_dependencies", (void**)&pdeps)))
        {
            xaml_char_t const* const* arr;
            XAML_RETURN_IF_FAILED(pdeps(&arr));
            for (size_t i = 0; arr[i]; i++)
            {
                XAML_RETURN_IF_FAILED(xaml_meta_context::add_module(arr[i]));
            }
        }
        return XAML_S_OK;
    }

    xaml_result XAML_CALL get_namespace(xaml_string* xml_ns, xaml_string** ptr) noexcept override
    {
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(m_namespace->lookup(xml_ns, &item));
        return item->query(ptr);
    }

    xaml_result XAML_CALL add_namespace(xaml_string* xml_ns, xaml_string* ns) noexcept override
    {
        bool replaced;
        return m_namespace->insert(xml_ns, ns, &replaced);
    }

    xaml_result XAML_CALL get_types(xaml_map_view** ptr) noexcept override
    {
        return m_type_info_map->query(ptr);
    }

    xaml_result XAML_CALL get_type(xaml_guid const& type, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        xaml_ptr<xaml_object> value;
        XAML_RETURN_IF_FAILED(m_type_info_map->lookup(key.get(), &value));
        return value->query(ptr);
    }

    xaml_result XAML_CALL get_type_by_name(xaml_string* name, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(m_name_info_map->lookup(name, &item));
        return item->query(ptr);
    }

    xaml_result XAML_CALL get_type_by_namespace_name(xaml_string* ns, xaml_string* name, xaml_reflection_info** ptr) noexcept override
    {
        xaml_ptr<xaml_string> real_name;
        XAML_RETURN_IF_FAILED(get_name_by_namespace_name(ns, name, &real_name));
        return get_type_by_name(real_name.get(), ptr);
    }

    xaml_result XAML_CALL get_name_by_namespace_name(xaml_string* ns, xaml_string* name, xaml_string** ptr) noexcept override
    {
        xaml_ptr<xaml_string> real_ns = ns;
        xaml_ptr<xaml_object> item;
        if (XAML_SUCCEEDED(m_namespace->lookup(ns, &item)))
        {
            real_ns = nullptr;
            XAML_RETURN_IF_FAILED(item->query(&real_ns));
        }
        xaml_std_string_view_t ns_view;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(real_ns.get(), ns_view));
        xaml_std_string_view_t name_view;
        XAML_RETURN_IF_FAILED(xaml_unbox_value(name, name_view));
        xaml_ptr<xaml_string> real_name;
        XAML_RETURN_IF_FAILED(xaml_string_new((xaml_std_string_t)ns_view + U("_") + (xaml_std_string_t)name_view, &real_name));
        return real_name->query(ptr);
    }

    xaml_result XAML_CALL add_type(xaml_reflection_info* info) noexcept override
    {
        xaml_guid type;
        XAML_RETURN_IF_FAILED(info->get_type(&type));
        xaml_ptr<xaml_string> name;
        XAML_RETURN_IF_FAILED(info->get_name(&name));
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        bool replaced;
        XAML_RETURN_IF_FAILED(m_type_info_map->insert(key.get(), info, &replaced));
        return m_name_info_map->insert(name.get(), info, &replaced);
    }

    xaml_result XAML_CALL get_basic_type(xaml_guid const& type, xaml_string** ptr) noexcept
    {
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        xaml_ptr<xaml_object> value;
        XAML_RETURN_IF_FAILED(m_basic_type_info_map->lookup(key.get(), &value));
        return value->query(ptr);
    }

    xaml_result XAML_CALL add_basic_type(xaml_guid const& type, xaml_string* name) noexcept
    {
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(xaml_box_value(type, &key));
        bool replaced;
        return m_basic_type_info_map->insert(key.get(), name, &replaced);
    }

    static xaml_result XAML_CALL get_property_changed_event_name(xaml_ptr<xaml_string> const& name, xaml_string** ptr) noexcept
    {
        xaml_std_string_view_t name_view;
        XAML_RETURN_IF_FAILED(to_string_view_t(name, name_view));
        return xaml_string_new((xaml_std_string_t)name_view + U("_changed"), ptr);
    }

    xaml_result XAML_CALL bind(xaml_object* target, xaml_string* target_prop, xaml_object* source, xaml_string* source_prop, xaml_binding_mode mode) noexcept override
    {
        xaml_ptr<xaml_type_info> target_type;
        {
            xaml_guid id;
            XAML_RETURN_IF_FAILED(target->get_guid(&id));
            xaml_ptr<xaml_reflection_info> info;
            XAML_RETURN_IF_FAILED(get_type(id, &info));
            XAML_RETURN_IF_FAILED(info->query(&target_type));
        }
        xaml_ptr<xaml_type_info> source_type;
        {
            xaml_guid id;
            XAML_RETURN_IF_FAILED(source->get_guid(&id));
            xaml_ptr<xaml_reflection_info> info;
            XAML_RETURN_IF_FAILED(get_type(id, &info));
            XAML_RETURN_IF_FAILED(info->query(&source_type));
        }
        xaml_ptr<xaml_property_info> targetp;
        XAML_RETURN_IF_FAILED(target_type->get_property(target_prop, &targetp));
        xaml_ptr<xaml_event_info> targete;
        {
            xaml_ptr<xaml_string> name;
            XAML_RETURN_IF_FAILED(get_property_changed_event_name(target_prop, &name));
            XAML_RETURN_IF_FAILED(target_type->get_event(name.get(), &targete));
        }
        xaml_ptr<xaml_property_info> sourcep;
        XAML_RETURN_IF_FAILED(source_type->get_property(source_prop, &sourcep));
        xaml_ptr<xaml_event_info> sourcee;
        {
            xaml_ptr<xaml_string> name;
            XAML_RETURN_IF_FAILED(get_property_changed_event_name(source_prop, &name));
            XAML_RETURN_IF_FAILED(source_type->get_event(name.get(), &sourcee));
        }
        if (mode & xaml_binding_one_way)
        {
            xaml_ptr<xaml_object> value;
            XAML_RETURN_IF_FAILED(sourcep->get(source, &value));
            XAML_RETURN_IF_FAILED(targetp->set(target, value.get()));
            xaml_ptr<xaml_delegate> callback;
            XAML_RETURN_IF_FAILED((xaml_delegate_new_noexcept<void>(
                [source, sourcep, target, targetp]() -> xaml_result {
                    xaml_ptr<xaml_object> value;
                    XAML_RETURN_IF_FAILED(sourcep->get(source, &value));
                    XAML_RETURN_IF_FAILED(targetp->set(target, value.get()));
                    return XAML_S_OK;
                },
                &callback)));
            int32_t token;
            XAML_RETURN_IF_FAILED(sourcee->add(source, callback.get(), &token));
        }
        if (mode & xaml_binding_one_way_to_source)
        {
            xaml_ptr<xaml_object> value;
            XAML_RETURN_IF_FAILED(targetp->get(target, &value));
            XAML_RETURN_IF_FAILED(sourcep->set(source, value.get()));
            xaml_ptr<xaml_delegate> callback;
            XAML_RETURN_IF_FAILED((xaml_delegate_new_noexcept<void>(
                [source, sourcep, target, targetp]() -> xaml_result {
                    xaml_ptr<xaml_object> value;
                    XAML_RETURN_IF_FAILED(targetp->get(target, &value));
                    XAML_RETURN_IF_FAILED(sourcep->set(source, value.get()));
                    return XAML_S_OK;
                },
                &callback)));
            int32_t token;
            XAML_RETURN_IF_FAILED(targete->add(target, callback.get(), &token));
        }
        return XAML_S_OK;
    }
};

xaml_result XAML_CALL xaml_meta_context_new(xaml_meta_context** ptr) noexcept
{
    return xaml_object_init<xaml_meta_context_impl>(ptr);
}
