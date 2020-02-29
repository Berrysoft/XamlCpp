#ifndef XAML_META_MACRO_HPP
#define XAML_META_MACRO_HPP

#include <xaml/meta/meta.hpp>

#define REGISTER_CLASS_DECL(ns, name)                                   \
    static ::std::unique_ptr<reflection_info> register_class() noexcept \
    {                                                                   \
        using self_type = name;                                         \
        auto ref = ::xaml::__make_reflection_info<self_type>(#ns, #name);

#define REGISTER_CLASS_END() \
    return ref;              \
    }

#define REGISTER_ENUM(ns, name) ctx.register_enum<name>(#ns, #name)

#define ADD_CTOR(...) ref->add_constructor<self_type, __VA_ARGS__>()
#define ADD_CTOR_DEF() ref->add_constructor<self_type>()

#define ADD_METHOD(name) ::xaml::__add_method_deduce_helper<self_type, decltype(&self_type::name)>{}(*ref, #name, &self_type::name)

#define ADD_STATIC_METHOD(name) ::xaml::__add_static_method_deduce_helper<self_type, decltype(&self_type::name)>{}(*ref, #name, &self_type::name)

#define PROP_RD(name, type) \
private:                    \
    type m_##name{};        \
                            \
public:                     \
    type get_##name() const { return m_##name; }

#define PROP(name, type) \
    PROP_RD(name, type)  \
    void set_##name(type value) { m_##name = value; }

#define PROP_REF(name, type) \
    PROP_RD(name, type)      \
    void set_##name(type const& value) { m_##name = value; }

#define PROP_CONSTEXPR_RD(name, type) \
private:                              \
    type m_##name{};                  \
                                      \
public:                               \
    constexpr type get_##name() const noexcept { return m_##name; }

#define PROP_CONSTEXPR(name, type) \
    PROP_CONSTEXPR_RD(name, type)  \
    void set_##name(type value) noexcept { m_##name = value; }

#define PROP_STRING_RD(name)     \
private:                         \
    ::xaml::string_t m_##name{}; \
                                 \
public:                          \
    ::xaml::string_view_t get_##name() const noexcept { return m_##name; }

#define PROP_STRING(name) \
    PROP_STRING_RD(name)  \
    void set_##name(::xaml::string_view_t value) noexcept { m_##name = (::xaml::string_t)value; }

#define ADD_PROP_TYPE(name, type) ref->add_property<type>(                                        \
    #name,                                                                                        \
    ::std::function<::std::any(::xaml::meta_class*)>(                                             \
        [](::xaml::meta_class* self) -> ::std::any {                                              \
            return ((self_type*)self)->get_##name();                                              \
        }),                                                                                       \
    ::std::function<void(::xaml::meta_class*, ::std::any)>(                                       \
        [](::xaml::meta_class* self, ::std::any value) -> void {                                  \
            ((self_type*)self)->set_##name(::xaml::value_converter_traits<type>::convert(value)); \
        }))

#define __GET_PROP_TYPE(name) decltype(::std::declval<self_type*>()->get_##name())

#define ADD_PROP(name) ADD_PROP_TYPE(name, __GET_PROP_TYPE(name))

#define ADD_ATTACH_PROP_TYPE(name, ctype, rtype) ref->add_property<rtype>(                              \
    #name,                                                                                              \
    ::std::function<::std::any(::xaml::meta_class*)>(                                                   \
        [](::xaml::meta_class* self) -> ::std::any {                                                    \
            return self_type::get_##name((ctype)*self);                                                 \
        }),                                                                                             \
    ::std::function<void(::xaml::meta_class*, ::std::any)>(                                             \
        [](::xaml::meta_class* self, ::std::any value) -> void {                                        \
            self_type::set_##name((ctype)*self, ::xaml::value_converter_traits<rtype>::convert(value)); \
        }),                                                                                             \
    true)

#define __GET_ATTACH_PROP_TYPE(name, ctype) decltype(self_type::get_##name(::std::declval<ctype>()))

#define ADD_ATTACH_PROP(name, ctype) ADD_ATTACH_PROP_TYPE(name, ctype, __GET_ATTACH_PROP_TYPE(name, ctype))

#define ADD_COLLECTION_PROP(name, type) ref->add_collection_property<type>(                          \
    #name,                                                                                           \
    ::std::function<void(::xaml::meta_class*, ::std::any)>(                                          \
        [](::xaml::meta_class* self, ::std::any value) -> void {                                     \
            ((self_type*)self)->add_##name(::xaml::value_converter_traits<type>::convert(value));    \
        }),                                                                                          \
    ::std::function<void(::xaml::meta_class*, ::std::any)>(                                          \
        [](::xaml::meta_class* self, ::std::any value) -> void {                                     \
            ((self_type*)self)->remove_##name(::xaml::value_converter_traits<type>::convert(value)); \
        }))

#define ADD_ATTACH_COLLECTION_PROP(name, ctype, type) ref->add_collection_property<type>(                 \
    #name,                                                                                                \
    ::std::function<void(::xaml::meta_class*, ::std::any)>(                                               \
        [](::xaml::meta_class* self, ::std::any value) -> void {                                          \
            self_type::add_##name((ctype*)self, ::xaml::value_converter_traits<type>::convert(value));    \
        }),                                                                                               \
    ::std::function<void(::xaml::meta_class*, ::std::any)>(                                               \
        [](::xaml::meta_class* self, ::std::any value) -> void {                                          \
            self_type::remove_##name((ctype*)self, ::xaml::value_converter_traits<type>::convert(value)); \
        }),                                                                                               \
    true)

#define EVENT(name, ...)                                                                       \
protected:                                                                                     \
    ::xaml::event<__VA_ARGS__> m_##name{};                                                     \
                                                                                               \
public:                                                                                        \
    ::std::size_t add_##name(::std::function<void(__VA_ARGS__)>&& f) { return m_##name += f; } \
    void remove_##name(::std::size_t token) { m_##name -= token; }

#define ADD_EVENT(name)                                                                               \
    ::xaml::__add_event_deduce_helper<self_type, decltype(::std::declval<self_type*>()->m_##name)>{}( \
        *ref, #name,                                                                                  \
        [](::xaml::meta_class* self, auto f) -> ::std::size_t {                                       \
            return ((self_type*)self)->add_##name(::std::move(f));                                    \
        },                                                                                            \
        [](::xaml::meta_class* self, std::size_t token) -> void {                                     \
            ((self_type*)self)->remove_##name(token);                                                 \
        },                                                                                            \
        &self_type::m_##name)

#define PROP_EVENT(name, type)                \
    PROP_RD(name, type)                       \
    void set_##name(type value)               \
    {                                         \
        if (m_##name != value)                \
        {                                     \
            m_##name = value;                 \
            m_##name##_changed(*this, value); \
        }                                     \
    }

#define PROP_REF_EVENT(name, type)            \
    PROP_RD(name, type)                       \
    void set_##name(type const& value)        \
    {                                         \
        if (m_##name != value)                \
        {                                     \
            m_##name = value;                 \
            m_##name##_changed(*this, value); \
        }                                     \
    }

#define PROP_CONSTEXPR_EVENT(name, type)      \
    PROP_CONSTEXPR_RD(name, type)             \
    void set_##name(type value)               \
    {                                         \
        if (m_##name != value)                \
        {                                     \
            m_##name = value;                 \
            m_##name##_changed(*this, value); \
        }                                     \
    }

#define PROP_STRING_EVENT(name)                  \
    PROP_STRING_RD(name)                         \
    void set_##name(::xaml::string_view_t value) \
    {                                            \
        if (m_##name != value)                   \
        {                                        \
            m_##name = (::xaml::string_t)value;  \
            m_##name##_changed(*this, m_##name); \
        }                                        \
    }

#define ADD_PROP_EVENT(name) \
    ADD_PROP(name);          \
    ADD_EVENT(name##_changed)

#define ADD_DEF_PROP(name) ref->set_attribute(::std::make_unique<::xaml::default_property>(#name))

#endif // !XAML_META_MACRO_HPP
