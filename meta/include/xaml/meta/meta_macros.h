#ifndef XAML_META_META_MACROS_HPP
#define XAML_META_META_MACROS_HPP

#include <xaml/delegate.h>
#include <xaml/meta/collection_property_info.h>
#include <xaml/meta/default_property.h>
#include <xaml/meta/enum_info.h>
#include <xaml/meta/event_info.h>
#include <xaml/meta/method_info.h>
#include <xaml/meta/property_info.h>
#include <xaml/meta/type_info.h>
#include <xaml/object.h>

#define XAML_PROP(name, type, gtype, stype) \
    XAML_METHOD(get_##name, type, gtype);   \
    XAML_METHOD(set_##name, type, stype)

#define XAML_CPROP(name, type, atype, rtype) \
    XAML_METHOD(add_##name, type, atype);    \
    XAML_METHOD(remove_##name, type, rtype)

#define XAML_EVENT(name, type, ts, te)                                                 \
    XAML_METHOD(add_##name, type, XAML_DELEGATE_T_T_NAME(ts, te)*, XAML_STD int32_t*); \
    XAML_METHOD(remove_##name, type, XAML_STD int32_t)

#ifdef __cplusplus
    #define XAML_PROP_IMPL_BASE(name, vtype, gtype)          \
        vtype m_##name{};                                    \
                                                             \
        xaml_result XAML_CALL get_##name(gtype ptr) noexcept \
        {                                                    \
            *ptr = m_##name;                                 \
            return XAML_S_OK;                                \
        }

    #define XAML_PROP_IMPL(name, vtype, gtype, stype)          \
        XAML_PROP_IMPL_BASE(name, vtype, gtype)                \
        xaml_result XAML_CALL set_##name(stype value) noexcept \
        {                                                      \
            m_##name = value;                                  \
            return XAML_S_OK;                                  \
        }

    #define XAML_PROP_EVENT_IMPL(name, vtype, gtype, stype)                \
        XAML_PROP_IMPL_BASE(name, vtype, gtype)                            \
        xaml_result XAML_CALL set_##name(stype value) noexcept             \
        {                                                                  \
            if (m_##name != value)                                         \
            {                                                              \
                m_##name = value;                                          \
                return m_##name##_changed->invoke(m_outer_this, m_##name); \
            }                                                              \
            return XAML_S_OK;                                              \
        }

    #define XAML_PROP_INTERNAL_IMPL_BASE(name, gtype)        \
        xaml_result XAML_CALL get_##name(gtype ptr) noexcept \
        {                                                    \
            return this->m_internal.get_##name(ptr);         \
        }

    #define XAML_PROP_INTERNAL_IMPL(name, gtype, stype)        \
        XAML_PROP_INTERNAL_IMPL_BASE(name, gtype)              \
        xaml_result XAML_CALL set_##name(stype value) noexcept \
        {                                                      \
            return this->m_internal.set_##name(value);         \
        }

    #define XAML_PROP_PTR_IMPL_BASE(name, type)               \
        xaml_ptr<type> m_##name{ nullptr };                   \
                                                              \
        xaml_result XAML_CALL get_##name(type** ptr) noexcept \
        {                                                     \
            return m_##name.query(ptr);                       \
        }

    #define XAML_PROP_PTR_IMPL(name, type)                     \
        XAML_PROP_PTR_IMPL_BASE(name, type)                    \
        xaml_result XAML_CALL set_##name(type* value) noexcept \
        {                                                      \
            m_##name = value;                                  \
            return XAML_S_OK;                                  \
        }

    #define XAML_PROP_PTR_EVENT_IMPL(name, type)                           \
        XAML_PROP_PTR_IMPL_BASE(name, type)                                \
        xaml_result XAML_CALL set_##name(type* value) noexcept             \
        {                                                                  \
            if (m_##name.get() != value)                                   \
            {                                                              \
                m_##name = value;                                          \
                return m_##name##_changed->invoke(m_outer_this, m_##name); \
            }                                                              \
            return XAML_S_OK;                                              \
        }

    #define XAML_PROP_PTR_INTERNAL_IMPL_BASE(name, type) XAML_PROP_INTERNAL_IMPL_BASE(name, type**)

    #define XAML_PROP_PTR_INTERNAL_IMPL(name, type) XAML_PROP_INTERNAL_IMPL(name, type**, type*)

    #define XAML_PROP_STRING_EVENT_IMPL(name)                                   \
        XAML_PROP_PTR_IMPL_BASE(name, xaml_string)                              \
        xaml_result XAML_CALL set_##name(xaml_string* value) noexcept           \
        {                                                                       \
            bool equal = false;                                                 \
            XAML_RETURN_IF_FAILED(xaml_string_equals(m_##name, value, &equal)); \
            if (!equal)                                                         \
            {                                                                   \
                m_##name = value;                                               \
                return m_##name##_changed->invoke(m_outer_this, m_##name);      \
            }                                                                   \
            return XAML_S_OK;                                                   \
        }

    #define XAML_CPROP_INTERNAL_IMPL(name, atype, rtype)          \
        xaml_result XAML_CALL add_##name(atype value) noexcept    \
        {                                                         \
            return this->m_internal.add_##name(value);            \
        }                                                         \
        xaml_result XAML_CALL remove_##name(rtype value) noexcept \
        {                                                         \
            return this->m_internal.remove_##name(value);         \
        }

    #define XAML_EVENT_IMPL(name, ts, te)                                                               \
        xaml_ptr<xaml_event<ts, te>> m_##name;                                                          \
                                                                                                        \
        xaml_result XAML_CALL add_##name(xaml_delegate<ts, te>* handler, std::int32_t* ptoken) noexcept \
        {                                                                                               \
            return m_##name->add(handler, ptoken);                                                      \
        }                                                                                               \
        xaml_result XAML_CALL remove_##name(std::int32_t token) noexcept                                \
        {                                                                                               \
            return m_##name->remove(token);                                                             \
        }

    #define XAML_EVENT_INTERNAL_IMPL(name, ts, te)                                                      \
        xaml_result XAML_CALL add_##name(xaml_delegate<ts, te>* handler, std::int32_t* ptoken) noexcept \
        {                                                                                               \
            return this->m_internal.add_##name(handler, ptoken);                                        \
        }                                                                                               \
        xaml_result XAML_CALL remove_##name(std::int32_t token) noexcept                                \
        {                                                                                               \
            return this->m_internal.remove_##name(token);                                               \
        }

    #define XAML_TYPE_INFO_NEW(type, file)                                \
        using self_type = type;                                           \
        xaml_ptr<xaml_string> __type_name;                                \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#type), &__type_name));   \
        xaml_ptr<xaml_string> __include_file;                             \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(file), &__include_file)); \
        xaml_ptr<xaml_type_info_registration> __info;                     \
        XAML_RETURN_IF_FAILED(xaml_type_info_registration_new<type>(__type_name, __include_file, &__info))

    #define XAML_TYPE_INFO_ADD_CTOR(ctor)                                      \
        do                                                                     \
        {                                                                      \
            xaml_ptr<xaml_constructor_info> __ctor;                            \
            XAML_RETURN_IF_FAILED((xaml_constructor_info_new(ctor, &__ctor))); \
            __info->set_constructor(__ctor);                                   \
        } while (0)

    #define XAML_TYPE_INFO_ADD_METHOD(method, ...)                                                                                      \
        do                                                                                                                              \
        {                                                                                                                               \
            xaml_ptr<xaml_string> __method_name;                                                                                        \
            XAML_RETURN_IF_FAILED(xaml_string_new(U(#method), &__method_name));                                                         \
            xaml_ptr<xaml_method_info> __method_info;                                                                                   \
            XAML_RETURN_IF_FAILED((xaml_method_info_new<__VA_ARGS__>(__method_name, xaml_mem_fn(&self_type::method), &__method_info))); \
            XAML_RETURN_IF_FAILED(__info->add_method(__method_info));                                                                   \
        } while (0)

    #define XAML_TYPE_INFO_ADD_PROP(prop, vtype)                                                                                               \
        do                                                                                                                                     \
        {                                                                                                                                      \
            xaml_ptr<xaml_string> __prop_name;                                                                                                 \
            XAML_RETURN_IF_FAILED(xaml_string_new(U(#prop), &__prop_name));                                                                    \
            xaml_ptr<xaml_property_info> __prop_info;                                                                                          \
            XAML_RETURN_IF_FAILED((xaml_property_info_new<vtype>(__prop_name, &self_type::get_##prop, &self_type::set_##prop, &__prop_info))); \
            XAML_RETURN_IF_FAILED(__info->add_property(__prop_info));                                                                          \
        } while (0)

    #define XAML_TYPE_INFO_ADD_PROP_RD(prop, vtype)                                                                    \
        do                                                                                                             \
        {                                                                                                              \
            xaml_ptr<xaml_string> __prop_name;                                                                         \
            XAML_RETURN_IF_FAILED(xaml_string_new(U(#prop), &__prop_name));                                            \
            xaml_ptr<xaml_property_info> __prop_info;                                                                  \
            XAML_RETURN_IF_FAILED((xaml_property_info_new<vtype>(__prop_name, &self_type::get_##prop, &__prop_info))); \
            XAML_RETURN_IF_FAILED(__info->add_property(__prop_info));                                                  \
        } while (0)

    #define XAML_TYPE_INFO_ADD_APROP(type, prop, vtype)                                                                              \
        do                                                                                                                           \
        {                                                                                                                            \
            xaml_ptr<xaml_string> __prop_name;                                                                                       \
            XAML_RETURN_IF_FAILED(xaml_string_new(U(#prop), &__prop_name));                                                          \
            xaml_ptr<xaml_property_info> __prop_info;                                                                                \
            XAML_RETURN_IF_FAILED((xaml_property_info_new<vtype>(__prop_name, type##_get_##prop, type##_set_##prop, &__prop_info))); \
            XAML_RETURN_IF_FAILED(__info->add_property(__prop_info));                                                                \
        } while (0)

    #define XAML_TYPE_INFO_ADD_CPROP(prop, vtype)                                                                                                            \
        do                                                                                                                                                   \
        {                                                                                                                                                    \
            xaml_ptr<xaml_string> __prop_name;                                                                                                               \
            XAML_RETURN_IF_FAILED(xaml_string_new(U(#prop), &__prop_name));                                                                                  \
            xaml_ptr<xaml_collection_property_info> __prop_info;                                                                                             \
            XAML_RETURN_IF_FAILED((xaml_collection_property_info_new<vtype>(__prop_name, &self_type::add_##prop, &self_type::remove_##prop, &__prop_info))); \
            XAML_RETURN_IF_FAILED(__info->add_collection_property(__prop_info));                                                                             \
        } while (0)

    #define XAML_TYPE_INFO_ADD_EVENT(event)                                                                                               \
        do                                                                                                                                \
        {                                                                                                                                 \
            xaml_ptr<xaml_string> __event_name;                                                                                           \
            XAML_RETURN_IF_FAILED(xaml_string_new(U(#event), &__event_name));                                                             \
            xaml_ptr<xaml_event_info> __event_info;                                                                                       \
            XAML_RETURN_IF_FAILED(xaml_event_info_new(__event_name, &self_type::add_##event, &self_type::remove_##event, &__event_info)); \
            XAML_RETURN_IF_FAILED(__info->add_event(__event_info));                                                                       \
        } while (0)

    #define XAML_TYPE_INFO_ADD_PROP_EVENT(prop, vtype) \
        XAML_TYPE_INFO_ADD_PROP(prop, vtype);          \
        XAML_TYPE_INFO_ADD_EVENT(prop##_changed)

    #define XAML_TYPE_INFO_ADD_DEF_PROP(name)                                           \
        do                                                                              \
        {                                                                               \
            xaml_ptr<xaml_string> __prop_name;                                          \
            XAML_RETURN_IF_FAILED(xaml_string_new(U(#name), &__prop_name));             \
            xaml_ptr<xaml_default_property> __def_prop;                                 \
            XAML_RETURN_IF_FAILED(xaml_default_property_new(__prop_name, &__def_prop)); \
            XAML_RETURN_IF_FAILED(__info->add_attribute(__def_prop.get()));             \
        } while (0)

    #define XAML_ENUM_INFO_NEW(type, file)                                \
        using self_type = type;                                           \
        xaml_ptr<xaml_string> __type_name;                                \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#type), &__type_name));   \
        xaml_ptr<xaml_string> __include_file;                             \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(file), &__include_file)); \
        xaml_ptr<xaml_enum_info> __info;                                  \
        XAML_RETURN_IF_FAILED(xaml_enum_info_new<type>(__type_name, __include_file, __map, &__info))

    #define XAML_ENUM_INFO_MAP_NEW()                                           \
        xaml_ptr<xaml_map> __map;                                              \
        do                                                                     \
        {                                                                      \
            xaml_ptr<xaml_hasher> __hasher;                                    \
            XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&__hasher));      \
            XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(__hasher, &__map)); \
        } while (0)

    #define XAML_ENUM_INFO_ADD(name, value)                                 \
        do                                                                  \
        {                                                                   \
            xaml_ptr<xaml_string> __name;                                   \
            XAML_RETURN_IF_FAILED(xaml_string_new(U(name), &__name));       \
            xaml_ptr<xaml_object> __box;                                    \
            XAML_RETURN_IF_FAILED(xaml_box_value(value, &__box));           \
            bool replaced;                                                  \
            XAML_RETURN_IF_FAILED(__map->insert(__name, __box, &replaced)); \
        } while (0)

    #define XAML_ENUM_INFO_ADD2(type, name) XAML_ENUM_INFO_ADD(#name, type##_##name)

    #define XAML_META_CONTEXT_ADD_BASIC_TYPE(type)                                                             \
        do                                                                                                     \
        {                                                                                                      \
            xaml_ptr<xaml_string> __name;                                                                      \
            XAML_RETURN_IF_FAILED(xaml_string_new(U(#type), &__name));                                         \
            xaml_ptr<xaml_basic_type_info> __info;                                                             \
            XAML_RETURN_IF_FAILED(xaml_basic_type_info_new(xaml_type_guid_v<type>, __name, nullptr, &__info)); \
            XAML_RETURN_IF_FAILED(ctx->add_type(__info));                                                      \
        } while (0)

#endif // __cplusplus

#endif // !XAML_META_META_MACROS_HPP
