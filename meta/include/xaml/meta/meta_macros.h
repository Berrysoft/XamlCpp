#ifndef XAML_META_META_MACROS_H
#define XAML_META_META_MACROS_H

#include <xaml/delegate.h>
#include <xaml/object.h>

#define XAML_PROP(name, type, gtype, stype) \
    XAML_METHOD(get_##name, type, gtype);   \
    XAML_METHOD(set_##name, type, stype)

#define XAML_PROP_IMPL_BASE(name, vtype, gtype)                   \
protected:                                                        \
    vtype m_##name;                                               \
                                                                  \
public:                                                           \
    xaml_result XAML_CALL get_##name(gtype ptr) noexcept override \
    {                                                             \
        *ptr = m_##name;                                          \
        return XAML_S_OK;                                         \
    }

#define XAML_PROP_IMPL(name, vtype, gtype, stype)                   \
    XAML_PROP_IMPL_BASE(name, vtype, gtype)                         \
    xaml_result XAML_CALL set_##name(stype value) noexcept override \
    {                                                               \
        m_##name = value;                                           \
        return XAML_S_OK;                                           \
    }

#define XAML_PROP_EVENT_IMPL(name, vtype, gtype, stype)             \
    XAML_PROP_IMPL_BASE(name, vtype, gtype)                         \
    xaml_result XAML_CALL set_##name(stype value) noexcept override \
    {                                                               \
        if (m_##name != value)                                      \
        {                                                           \
            m_##name = value;                                       \
            return on_##name##_changed(this, m_##name);             \
        }                                                           \
        return XAML_S_OK;                                           \
    }

#define XAML_PROP_PTR_IMPL_BASE(name, type)                        \
protected:                                                         \
    xaml_ptr<type> m_##name;                                       \
                                                                   \
public:                                                            \
    xaml_result XAML_CALL get_##name(type** ptr) noexcept override \
    {                                                              \
        m_##name.add_ref_to(ptr);                                  \
        return XAML_S_OK;                                          \
    }

#define XAML_PROP_PTR_IMPL(name, type)                              \
    XAML_PROP_PTR_IMPL_BASE(name, type)                             \
    xaml_result XAML_CALL set_##name(type* value) noexcept override \
    {                                                               \
        m_##name = value;                                           \
        return XAML_S_OK;                                           \
    }

#define XAML_PROP_PTR_EVENT_IMPL(name, type)                        \
    XAML_PROP_PTR_IMPL_BASE(name, type)                             \
    xaml_result XAML_CALL set_##name(type* value) noexcept override \
    {                                                               \
        if (m_##name != value)                                      \
        {                                                           \
            m_##name = value;                                       \
            return on_##name##_changed(this, m_##name);             \
        }                                                           \
        return XAML_S_OK;                                           \
    }

#define XAML_EVENT(name, type)                                        \
    XAML_METHOD(add_##name, type, xaml_delegate*, XAML_CSTD size_t*); \
    XAML_METHOD(remove_##name, type, XAML_CSTD size_t)

#define XAML_EVENT_IMPL(name)                                                                       \
protected:                                                                                          \
    xaml_ptr<xaml_event> m_##name;                                                                  \
    template <typename... Args>                                                                     \
    xaml_result on_##name(Args&&... args) noexcept                                                  \
    {                                                                                               \
        xaml_ptr<xaml_vector_view> invoke_args;                                                     \
        XAML_RETURN_IF_FAILED(xaml_delegate_pack_args(&invoke_args, std::forward<Args>(args)...));  \
        xaml_ptr<xaml_object> res;                                                                  \
        return m_##name->invoke(invoke_args.get(), &res);                                           \
    }                                                                                               \
                                                                                                    \
public:                                                                                             \
    xaml_result XAML_CALL add_##name(xaml_delegate* handler, std::size_t* ptoken) noexcept override \
    {                                                                                               \
        return m_##name->add(handler, ptoken);                                                      \
    }                                                                                               \
    xaml_result XAML_CALL remove_##name(std::size_t token) noexcept override                        \
    {                                                                                               \
        return m_##name->remove(token);                                                             \
    }

#endif // !XAML_META_META_MACROS_H
