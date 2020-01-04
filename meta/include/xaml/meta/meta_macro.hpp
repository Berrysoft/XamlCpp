#ifndef XAML_META_MACRO_HPP
#define XAML_META_MACRO_HPP

#include <xaml/meta/meta.hpp>

#define REGISTER_TYPE(ns, name) \
    using self_type = name;     \
    ::xaml::register_type<self_type>(#ns, #name)

#define ADD_CTOR(...) ::xaml::add_constructor<self_type, __VA_ARGS__>()
#define ADD_CTOR_DEF() ::xaml::add_constructor<self_type>()

#define ADD_METHOD(name) ::xaml::add_method<self_type>(#name, &self_type::name)

#define ADD_STATIC_METHOD(name) ::xaml::add_static_method<self_type>(#name, self_type::name)

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

#define ADD_PROP_TYPE(name, type) ::xaml::add_property_ex<self_type, type>(#name, ::std::function<type(self_type*)>([](self_type* self) -> type { return self->get_##name(); }), ::std::function<void(self_type*, type)>([](self_type* self, type value) -> void { self->set_##name(value); }))
#define ADD_PROP_TYPE_RD(name, type) ::xaml::add_property_read_ex<self_type, type>(#name, ::std::function<type(self_type*)>([](self_type* self) -> type { return self->get_##name(); }))

#define __GET_PROP_TYPE(name) decltype(::std::declval<self_type*>()->get_##name())

#define ADD_PROP(name) ADD_PROP_TYPE(name, __GET_PROP_TYPE(name))
#define ADD_PROP_RD(name) ADD_PROP_TYPE_RD(name, __GET_PROP_TYPE(name))

#define ADD_ATTACH_PROP_TYPE(name, ctype, rtype) ::xaml::add_attach_property<self_type, ctype, rtype>(#name, self_type::get_##name, self_type::set_##name)

#define __GET_ATTACH_PROP_TYPE(name, ctype) decltype(self_type::get_##name(::std::declval<ctype>()))

#define ADD_ATTACH_PROP(name, ctype) ADD_ATTACH_PROP_TYPE(name, ctype, __GET_ATTACH_PROP_TYPE(name, ctype))

#define EVENT(name, ...)                                                                                                         \
protected:                                                                                                                       \
    ::xaml::event<__VA_ARGS__> m_##name{};                                                                                       \
    ::xaml::event<__VA_ARGS__>& get_##name() { return m_##name; }                                                                \
                                                                                                                                 \
public:                                                                                                                          \
    typename ::xaml::event<__VA_ARGS__>::token_type add_##name(::std::function<void(__VA_ARGS__)>&& f) { return m_##name += f; } \
    void remove_##name(typename ::xaml::event<__VA_ARGS__>::token_type token) { m_##name -= token; }

#define ADD_EVENT(name)                                                    \
    ::xaml::__add_event_deduce_helper<self_type, __GET_PROP_TYPE(name)>{}( \
        #name, [](self_type * self, auto f) -> typename event_info::token_type { return self->add_##name(::std::move(f)); }, [](self_type* self, typename event_info::token_type token) -> void { self->remove_##name(token); }, [](self_type* self) -> __GET_PROP_TYPE(name) { return self->get_##name(); })

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

#define ADD_PROP_EVENT(name) \
    ADD_PROP(name);          \
    ADD_EVENT(name##_changed)

#define ADD_DEF_PROP(name) ::xaml::set_attribute(::std::type_index(typeid(self_type)), ::std::make_shared<::xaml::default_property>(#name))

#endif // !XAML_META_MACRO_HPP
