#ifndef XAML_META_META_MACROS_H
#define XAML_META_META_MACROS_H

#include <xaml/delegate.h>
#include <xaml/meta/collection_property_info.h>
#include <xaml/meta/event_info.h>
#include <xaml/meta/method_info.h>
#include <xaml/meta/property_info.h>
#include <xaml/meta/type_info.h>
#include <xaml/object.h>

#define XAML_PROP(name, type, gtype, stype) \
    XAML_METHOD(get_##name, type, gtype);   \
    XAML_METHOD(set_##name, type, stype)

#define XAML_PROP_IMPL_BASE(name, vtype, gtype)          \
protected:                                               \
    vtype m_##name;                                      \
                                                         \
public:                                                  \
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

#define XAML_PROP_EVENT_IMPL(name, vtype, gtype, stype)    \
    XAML_PROP_IMPL_BASE(name, vtype, gtype)                \
    xaml_result XAML_CALL set_##name(stype value) noexcept \
    {                                                      \
        if (m_##name != value)                             \
        {                                                  \
            m_##name = value;                              \
            return on_##name##_changed(this, m_##name);    \
        }                                                  \
        return XAML_S_OK;                                  \
    }

#define XAML_PROP_PTR_IMPL_BASE(name, type)               \
protected:                                                \
    xaml_ptr<type> m_##name;                              \
                                                          \
public:                                                   \
    xaml_result XAML_CALL get_##name(type** ptr) noexcept \
    {                                                     \
        m_##name.add_ref_to(ptr);                         \
        return XAML_S_OK;                                 \
    }

#define XAML_PROP_PTR_IMPL(name, type)                     \
    XAML_PROP_PTR_IMPL_BASE(name, type)                    \
    xaml_result XAML_CALL set_##name(type* value) noexcept \
    {                                                      \
        m_##name = value;                                  \
        return XAML_S_OK;                                  \
    }

#define XAML_PROP_PTR_EVENT_IMPL(name, type)               \
    XAML_PROP_PTR_IMPL_BASE(name, type)                    \
    xaml_result XAML_CALL set_##name(type* value) noexcept \
    {                                                      \
        if (m_##name != value)                             \
        {                                                  \
            m_##name = value;                              \
            return on_##name##_changed(this, m_##name);    \
        }                                                  \
        return XAML_S_OK;                                  \
    }

#define XAML_CPROP(name, type, atype, rtype) \
    XAML_METHOD(add_##name, type, atype);    \
    XAML_METHOD(remove_##name, type, rtype)

#define XAML_EVENT(name, type)                                        \
    XAML_METHOD(add_##name, type, xaml_delegate*, XAML_CSTD size_t*); \
    XAML_METHOD(remove_##name, type, XAML_CSTD size_t)

#define XAML_EVENT_IMPL(name)                                                                      \
protected:                                                                                         \
    xaml_ptr<xaml_event> m_##name;                                                                 \
    template <typename... Args>                                                                    \
    xaml_result on_##name(Args&&... args) noexcept                                                 \
    {                                                                                              \
        xaml_ptr<xaml_vector_view> invoke_args;                                                    \
        XAML_RETURN_IF_FAILED(xaml_delegate_pack_args(&invoke_args, std::forward<Args>(args)...)); \
        xaml_ptr<xaml_object> res;                                                                 \
        return m_##name->invoke(invoke_args.get(), &res);                                          \
    }                                                                                              \
                                                                                                   \
public:                                                                                            \
    xaml_result XAML_CALL add_##name(xaml_delegate* handler, std::size_t* ptoken) noexcept         \
    {                                                                                              \
        return m_##name->add(handler, ptoken);                                                     \
    }                                                                                              \
    xaml_result XAML_CALL remove_##name(std::size_t token) noexcept                                \
    {                                                                                              \
        return m_##name->remove(token);                                                            \
    }

#define XAML_TYPE_INFO_NEW(type, file)                                \
    using self_type = type;                                           \
    xaml_ptr<xaml_string> __type_name;                                \
    XAML_RETURN_IF_FAILED(xaml_string_new(U(#type), &__type_name));   \
    xaml_ptr<xaml_string> __include_file;                             \
    XAML_RETURN_IF_FAILED(xaml_string_new(U(file), &__include_file)); \
    xaml_ptr<xaml_type_info_registration> __info;                     \
    XAML_RETURN_IF_FAILED(xaml_type_info_registration_new<type>(__type_name.get(), __include_file.get(), &__info))

#define XAML_TYPE_INFO_ADD_CTOR(ctor)                                 \
    do                                                                \
    {                                                                 \
        xaml_ptr<xaml_delegate> __ctor;                               \
        XAML_RETURN_IF_FAILED(xaml_delegate_new<xaml_ptr<self_type>>( \
            []() -> xaml_ptr<self_type> {                             \
                xaml_ptr<self_type> __res;                            \
                XAML_THROW_IF_FAILED(ctor(&__res));                   \
                return __res;                                         \
            },                                                        \
            &__ctor));                                                \
        __info->set_constructor(__ctor.get());                        \
    } while (0)

#define XAML_TYPE_INFO_ADD_METHOD(method)                                                                     \
    do                                                                                                        \
    {                                                                                                         \
        xaml_ptr<xaml_string> __method_name;                                                                  \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#method), &__method_name));                                   \
        xaml_ptr<xaml_method_info> __method_info;                                                             \
        XAML_RETURN_IF_FAILED(xaml_method_info_new(__method_name.get(), &self_type::method, &__method_info)); \
        XAML_RETURN_IF_FAILED(__info->add_method(__method_info.get()));                                       \
    } while (0)

#define XAML_TYPE_INFO_ADD_PROP(prop)                                                                                                   \
    do                                                                                                                                  \
    {                                                                                                                                   \
        xaml_ptr<xaml_string> __prop_name;                                                                                              \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#prop), &__prop_name));                                                                 \
        xaml_ptr<xaml_property_info> __prop_info;                                                                                       \
        XAML_RETURN_IF_FAILED(xaml_property_info_new(__prop_name.get(), &self_type::get_##prop, &self_type::set_##prop, &__prop_info)); \
        XAML_RETURN_IF_FAILED(__info->add_property(__prop_info.get()));                                                                 \
    } while (0)

#define XAML_TYPE_INFO_ADD_PROP_RD(prop)                                                                        \
    do                                                                                                          \
    {                                                                                                           \
        xaml_ptr<xaml_string> __prop_name;                                                                      \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#prop), &__prop_name));                                         \
        xaml_ptr<xaml_property_info> __prop_info;                                                               \
        XAML_RETURN_IF_FAILED(xaml_property_info_new(__prop_name.get(), &self_type::get_##prop, &__prop_info)); \
        XAML_RETURN_IF_FAILED(__info->add_property(__prop_info.get()));                                         \
    } while (0)

#define XAML_TYPE_INFO_ADD_CPROP(prop)                                                                                                     \
    do                                                                                                                                     \
    {                                                                                                                                      \
        xaml_ptr<xaml_string> __prop_name;                                                                                                 \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#prop), &__prop_name));                                                                    \
        xaml_ptr<xaml_property_info> __prop_info;                                                                                          \
        XAML_RETURN_IF_FAILED(xaml_property_info_new(__prop_name.get(), &self_type::add_##prop, &self_type::remove_##prop, &__prop_info)); \
        XAML_RETURN_IF_FAILED(__info->add_collection_property(__prop_info.get()));                                                         \
    } while (0)

#define XAML_TYPE_INFO_ADD_EVENT(event)                                                                                                     \
    do                                                                                                                                      \
    {                                                                                                                                       \
        xaml_ptr<xaml_string> __event_name;                                                                                                 \
        XAML_RETURN_IF_FAILED(xaml_string_new(U(#event), &__event_name));                                                                   \
        xaml_ptr<xaml_event_info> __event_info;                                                                                             \
        XAML_RETURN_IF_FAILED(xaml_event_info_new(__event_name.get(), &self_type::add_##event, &self_type::remove_##event, &__event_info)); \
        XAML_RETURN_IF_FAILED(__info->add_event(__event_info.get()));                                                                       \
    } while (0)

#define XAML_TYPE_INFO_ADD_PROP_EVENT(prop) \
    XAML_TYPE_INFO_ADD_PROP(prop);          \
    XAML_TYPE_INFO_ADD_EVENT(prop##_changed)

#endif // !XAML_META_META_MACROS_H
