#ifndef XAML_META_MACRO_HPP
#define XAML_META_MACRO_HPP

#include <xaml/meta/meta.hpp>

#define REGISTER_TYPE() ::xaml::register_type<self_type>(self_type::class_name)

#define ADD_CTOR(...) ::xaml::add_constructor<self_type, ##__VA_ARGS__>()

#define ADD_METHOD(name) ::xaml::add_method<self_type>(#name, &self_type::name)

#define PROP_RD(name, type) \
private:                    \
    type m_##name;          \
                            \
public:                     \
    type get_##name() { return m_##name; }

#define PROP(name, type) \
    PROP_RD(name, type)  \
    void set_##name(type value) { m_##name = value; }

#define ADD_PROP_TYPE(name, type) ::xaml::add_property<self_type, type>(#name, &self_type::get_##name, &self_type::set_##name)
#define ADD_PROP_TYPE_RD(name, type) ::xaml::add_property_read<self_type, type>(#name, &self_type::get_##name)

#define __GET_PROP_TYPE(name) decltype(::std::declval<self_type*>()->get_##name())

#define ADD_PROP(name) ADD_PROP_TYPE(name, __GET_PROP_TYPE(name))
#define ADD_PROP_RD(name) ADD_PROP_TYPE_RD(name, __GET_PROP_TYPE(name))

#define EVENT(name, ...)                                                                                                         \
private:                                                                                                                         \
    ::xaml::event<__VA_ARGS__> m_##name;                                                                                         \
    ::xaml::event<__VA_ARGS__>& get_##name() { return m_##name; }                                                                \
                                                                                                                                 \
public:                                                                                                                          \
    typename ::xaml::event<__VA_ARGS__>::token_type add_##name(::std::function<void(__VA_ARGS__)>&& f) { return m_##name += f; } \
    void remove_##name(typename ::xaml::event<__VA_ARGS__>::token_type token) { m_##name -= token; }

#define ADD_EVENT(name)                                                                             \
    ::xaml::__add_event_deduce_helper<self_type, decltype(::std::declval<self_type>().m_##name)>{}( \
        #name, &self_type::add_##name, &self_type::remove_##name, &self_type::get_##name)

#define PROP_EVENT(name, type)                    \
    PROP_RD(name, type)                           \
    EVENT(name##_changed, self_type const&, type) \
    void set_##name(type value)                   \
    {                                             \
        if (m_##name != value)                    \
        {                                         \
            m_##value = value;                    \
            m_##name##_changed(*this, value);     \
        }                                         \
    }

#define ADD_PROP_EVENT(name) \
    ADD_PROP(name);          \
    ADD_EVENT(name##_changed)

#endif // !XAML_META_MACRO_HPP
