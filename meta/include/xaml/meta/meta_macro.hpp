#ifndef XAML_META_MACRO_HPP
#define XAML_META_MACRO_HPP

#include <xaml/meta/meta.hpp>

#define REGISTER_CLASS_DECL() static void register_class(::xaml::meta_context& ctx) noexcept

#define REGISTER_ENUM(ns, name) ctx.register_enum<name>(#ns, #name)

#define REGISTER_TYPE(ns, name) \
    using self_type = name;     \
    ctx.register_type<self_type>(#ns, #name)

#define ADD_CTOR(...) ctx.add_constructor<self_type, __VA_ARGS__>()
#define ADD_CTOR_DEF() ctx.add_constructor<self_type>()

#define ADD_METHOD(name) ctx.add_method<self_type>(#name, &self_type::name)

#define ADD_STATIC_METHOD(name) ctx.add_static_method<self_type>(#name, self_type::name)

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

#define ADD_PROP_TYPE(name, type) ctx.add_property_ex<self_type, type>(#name, ::std::function<type(self_type*)>([](self_type* self) -> type { return self->get_##name(); }), ::std::function<void(self_type*, type)>([](self_type* self, type value) -> void { self->set_##name(value); }))

#define __GET_PROP_TYPE(name) decltype(::std::declval<self_type*>()->get_##name())

#define ADD_PROP(name) ADD_PROP_TYPE(name, __GET_PROP_TYPE(name))

#define ADD_COLLECTION_PROP(name, type) ctx.add_collection_property<self_type, type>(#name, ::std::function<void(self_type*, type)>([](self_type* self, type value) -> void { self->add_##name(value); }), ::std::function<void(self_type*, type)>([](self_type* self, type value) -> void { self->remove_##name(value); }))

#define ADD_ATTACH_PROP_TYPE(name, ctype, rtype) ctx.add_attach_property<self_type, ctype, rtype>(#name, self_type::get_##name, self_type::set_##name)

#define __GET_ATTACH_PROP_TYPE(name, ctype) decltype(self_type::get_##name(::std::declval<ctype>()))

#define ADD_ATTACH_PROP(name, ctype) ADD_ATTACH_PROP_TYPE(name, ctype, __GET_ATTACH_PROP_TYPE(name, ctype))

#define ADD_ATTACH_COLLECTION_PROP(name, ctype, type) ctx.add_attach_collection_property<self, ctype, type>(#name, self_type::add_##name, self_type::remove_##name)

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
        ctx, #name, [](self_type * self, auto f) -> typename event_info::token_type { return self->add_##name(::std::move(f)); }, [](self_type* self, typename event_info::token_type token) -> void { self->remove_##name(token); }, [](self_type* self) -> __GET_PROP_TYPE(name) { return self->get_##name(); })

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

#define ADD_DEF_PROP(name) ctx.set_attribute(::std::type_index(typeid(self_type)), ::std::make_unique<::xaml::default_property>(#name))

#endif // !XAML_META_MACRO_HPP
