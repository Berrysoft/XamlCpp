#ifndef XAML_META_MACRO_HPP
#define XAML_META_MACRO_HPP

#include <xaml/meta/conv.hpp>
#include <xaml/meta/meta.hpp>

#define REGISTER_CLASS_DECL_FILE(ns, name, file)                                \
    static ::std::unique_ptr<::xaml::reflection_info> register_class() noexcept \
    {                                                                           \
        using self_type = ::ns::name;                                           \
        auto ref = ::std::make_unique<::xaml::reflection_info>(::xaml::type_guid_v<self_type>, #ns, #name, file);

#define REGISTER_CLASS_DECL(ns, name, prefix) REGISTER_CLASS_DECL_FILE(ns, name, prefix "/" #name ".hpp")
#define REGISTER_CLASS_DECL_NOFILE(ns, name) REGISTER_CLASS_DECL_FILE(ns, name, "")

#define REGISTER_CLASS_END() \
    return ref;              \
    }

#define REGISTER_ENUM(ns, name, file) ctx.register_type(::std::make_unique<::xaml::enum_reflection_info>(::xaml::type_guid_v<::xaml::box_value_t<::ns::name>>, #ns, #name, file))

#ifdef _MSC_VER
#define ADD_CTOR(...) ref->add_constructor<self_type, ##__VA_ARGS__>()
#else
#define ADD_CTOR(...) ref->add_constructor<self_type __VA_OPT__(, ) __VA_ARGS__>()
#endif // _MSC_VER

#define ADD_METHOD(name) ref->add_method(#name, ::xaml::make_type_erased_this_function(&self_type::name))

#define ADD_STATIC_METHOD(name) ref->add_static_method(#name, ::xaml::make_type_erased_function(&self_type::name))

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

#define ADD_PROP_TYPE(name, type) ref->add_property<type>(                                                                 \
    #name,                                                                                                                 \
    ::std::function<::std::shared_ptr<::xaml::meta_class>(::std::shared_ptr<::xaml::meta_class>)>(                         \
        [](::std::shared_ptr<::xaml::meta_class> self) -> ::std::shared_ptr<::xaml::meta_class> {                          \
            return ::xaml::box_value(::std::static_pointer_cast<self_type>(self)->get_##name());                           \
        }),                                                                                                                \
    ::std::function<void(::std::shared_ptr<::xaml::meta_class>, ::std::shared_ptr<::xaml::meta_class>)>(                   \
        [](::std::shared_ptr<::xaml::meta_class> self, ::std::shared_ptr<::xaml::meta_class> value) -> void {              \
            ::std::static_pointer_cast<self_type>(self)->set_##name(::xaml::value_converter_traits<type>::convert(value)); \
        }))

#define __GET_PROP_TYPE(name) decltype(::std::declval<self_type*>()->get_##name())

#define ADD_PROP(name) ADD_PROP_TYPE(name, __GET_PROP_TYPE(name))

#define ADD_ATTACH_PROP_TYPE(name, ctype, rtype) ref->add_property<rtype>(                                    \
    #name,                                                                                                    \
    ::std::function<::std::shared_ptr<::xaml::meta_class>(::std::shared_ptr<::xaml::meta_class>)>(            \
        [](::std::shared_ptr<::xaml::meta_class> self) -> ::std::shared_ptr<::xaml::meta_class> {             \
            return ::xaml::box_value(self_type::get_##name((ctype)*self));                                    \
        }),                                                                                                   \
    ::std::function<void(::std::shared_ptr<::xaml::meta_class>, ::std::shared_ptr<::xaml::meta_class>)>(      \
        [](::std::shared_ptr<::xaml::meta_class> self, ::std::shared_ptr<::xaml::meta_class> value) -> void { \
            self_type::set_##name((ctype)*self, ::xaml::value_converter_traits<rtype>::convert(value));       \
        }),                                                                                                   \
    true)

#define __GET_ATTACH_PROP_TYPE(name, ctype) decltype(self_type::get_##name(::std::declval<ctype>()))

#define ADD_ATTACH_PROP(name, ctype) ADD_ATTACH_PROP_TYPE(name, ctype, __GET_ATTACH_PROP_TYPE(name, ctype))

#define ADD_COLLECTION_PROP(name, type) ref->add_collection_property<type>(                                                   \
    #name,                                                                                                                    \
    ::std::function<void(::std::shared_ptr<::xaml::meta_class>, ::std::shared_ptr<::xaml::meta_class>)>(                      \
        [](::std::shared_ptr<::xaml::meta_class> self, ::std::shared_ptr<::xaml::meta_class> value) -> void {                 \
            ::std::static_pointer_cast<self_type>(self)->add_##name(::xaml::value_converter_traits<type>::convert(value));    \
        }),                                                                                                                   \
    ::std::function<void(::std::shared_ptr<::xaml::meta_class>, ::std::shared_ptr<::xaml::meta_class>)>(                      \
        [](::std::shared_ptr<::xaml::meta_class> self, ::std::shared_ptr<::xaml::meta_class> value) -> void {                 \
            ::std::static_pointer_cast<self_type>(self)->remove_##name(::xaml::value_converter_traits<type>::convert(value)); \
        }))

#define ADD_ATTACH_COLLECTION_PROP(name, ctype, type) ref->add_collection_property<type>(                     \
    #name,                                                                                                    \
    ::std::function<void(::std::shared_ptr<::xaml::meta_class>, ::std::shared_ptr<::xaml::meta_class>)>(      \
        [](::std::shared_ptr<::xaml::meta_class> self, ::std::shared_ptr<::xaml::meta_class> value) -> void { \
            self_type::add_##name((ctype*)self, ::xaml::value_converter_traits<type>::convert(value));        \
        }),                                                                                                   \
    ::std::function<void(::std::shared_ptr<::xaml::meta_class>, ::std::shared_ptr<::xaml::meta_class>)>(      \
        [](::std::shared_ptr<::xaml::meta_class> self, ::std::shared_ptr<::xaml::meta_class> value) -> void { \
            self_type::remove_##name((ctype*)self, ::xaml::value_converter_traits<type>::convert(value));     \
        }),                                                                                                   \
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
        [](::std::shared_ptr<::xaml::meta_class> self, auto f) -> ::std::size_t {                     \
            return ::std::static_pointer_cast<self_type>(self)->add_##name(::std::move(f));           \
        },                                                                                            \
        [](::std::shared_ptr<::xaml::meta_class> self, std::size_t token) -> void {                   \
            ::std::static_pointer_cast<self_type>(self)->remove_##name(token);                        \
        },                                                                                            \
        &self_type::m_##name)

#define __DETERMINE_NEQ_AND_RAISE_EVENT(name)                                                                       \
    if (m_##name != value)                                                                                          \
    {                                                                                                               \
        m_##name = value;                                                                                           \
        m_##name##_changed(::std::static_pointer_cast<::std::decay_t<decltype(*this)>>(shared_from_this()), value); \
    }

#define PROP_EVENT(name, type)                \
    PROP_RD(name, type)                       \
    void set_##name(type value)               \
    {                                         \
        __DETERMINE_NEQ_AND_RAISE_EVENT(name) \
    }

#define PROP_REF_EVENT(name, type)            \
    PROP_RD(name, type)                       \
    void set_##name(type const& value)        \
    {                                         \
        __DETERMINE_NEQ_AND_RAISE_EVENT(name) \
    }

#define PROP_CONSTEXPR_EVENT(name, type)      \
    PROP_CONSTEXPR_RD(name, type)             \
    void set_##name(type value)               \
    {                                         \
        __DETERMINE_NEQ_AND_RAISE_EVENT(name) \
    }

#define PROP_STRING_EVENT(name)                  \
    PROP_STRING_RD(name)                         \
    void set_##name(::xaml::string_view_t value) \
    {                                            \
        __DETERMINE_NEQ_AND_RAISE_EVENT(name)    \
    }

#define ADD_PROP_EVENT(name) \
    ADD_PROP(name);          \
    ADD_EVENT(name##_changed)

#define ADD_ATTR(expr) ref->set_attribute((expr))

#define ADD_DEF_PROP(name) ADD_ATTR(::std::make_shared<::xaml::default_property>(#name))

#endif // !XAML_META_MACRO_HPP
