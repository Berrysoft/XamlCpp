#ifndef XAML_META_HPP
#define XAML_META_HPP

#include <algorithm>
#include <any>
#include <array>
#include <functional>
#include <initializer_list>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <tuple>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <xaml/meta/conv.hpp>
#include <xaml/meta/event.hpp>
#include <xaml/utility.hpp>

namespace xaml
{
    // BASE TYPE

    // Base class of all classes which isn't inheritable and registered for reflection.
    struct meta_class : std::enable_shared_from_this<meta_class>
    {
        std::type_index this_type() const noexcept { return std::type_index(typeid(*this)); }
        virtual ~meta_class() {}
    };

    template <typename T>
    struct value_converter_traits<T*, std::enable_if_t<std::is_base_of_v<meta_class, T>>>
    {
        static T* convert(std::any value)
        {
            if (value.type() == typeid(T*))
            {
                return std::any_cast<T*>(value);
            }
            else if (value.type() == typeid(meta_class*))
            {
                return static_cast<T*>(std::any_cast<meta_class*>(value));
            }
            else
            {
                return {};
            }
        }
    };

    template <typename T>
    struct value_converter_traits<std::unique_ptr<T>, std::enable_if_t<std::is_base_of_v<meta_class, T>>>
    {
        static std::unique_ptr<T> convert(std::any value)
        {
            if (value.type() == typeid(std::unique_ptr<T>))
            {
                return std::move(std::any_cast<std::unique_ptr<T>>(value));
            }
            else if (value.type() == typeid(std::unique_ptr<meta_class>))
            {
                return std::unique_ptr<T>(std::any_cast<std::unique_ptr<meta_class>>(value).release());
            }
            else
            {
                return {};
            }
        }
    };

    template <typename T>
    struct value_converter_traits<std::shared_ptr<T>, std::enable_if_t<std::is_base_of_v<meta_class, T>>>
    {
        static std::shared_ptr<T> convert(std::any value)
        {
            if (value.type() == typeid(std::shared_ptr<T>))
            {
                return std::any_cast<std::shared_ptr<T>>(value);
            }
            else if (value.type() == typeid(std::shared_ptr<meta_class>))
            {
                return std::static_pointer_cast<T>(std::any_cast<std::shared_ptr<meta_class>>(value));
            }
            else
            {
                return {};
            }
        }
    };

    struct __type_index_wrapper
    {
        std::type_index type{ typeid(std::nullptr_t) };
    };

    // A type-erased function, for storage convinence.
    struct __type_erased_function
    {
        virtual bool is_same_arg_type(std::initializer_list<std::type_index> types) const noexcept = 0;
        virtual bool is_same_return_type(std::type_index type) const noexcept = 0;

        template <typename... Args>
        bool is_same_arg_type() const noexcept
        {
            return is_same_arg_type({ std::type_index(typeid(Args))... });
        }

        template <typename Return>
        bool is_same_return_type() const noexcept
        {
            return is_same_return_type(std::type_index(typeid(Return)));
        }

        virtual ~__type_erased_function() {}
    };

    template <typename T>
    struct __optional_return
    {
        using type = std::optional<T>;
    };

    template <>
    struct __optional_return<void>
    {
        using type = bool;
    };

    template <typename T>
    using __optional_return_t = typename __optional_return<T>::type;

    template <typename T>
    struct __remove_const_func
    {
        using type = T;
    };

    template <typename Return, typename... Args>
    struct __remove_const_func<Return(Args...) const>
    {
        using type = Return(Args...);
    };

    template <typename T>
    using __remove_const_func_t = typename __remove_const_func<T>::type;

    template <typename T>
    struct __type_erased_function_impl;

    template <typename Return, typename... Args>
    struct __type_erased_function_impl<Return(Args...)> : __type_erased_function
    {
    private:
        static inline bool is_same_arg_type_impl(std::initializer_list<std::type_index> lhs, std::initializer_list<std::type_index> rhs)
        {
            return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
        }

    public:
        bool is_same_arg_type(std::initializer_list<std::type_index> types) const noexcept override final
        {
            return is_same_arg_type_impl({ std::type_index(typeid(Args))... }, types);
        }
        bool is_same_return_type(std::type_index type) const noexcept override final
        {
            return type == std::type_index(typeid(Return));
        }
        ~__type_erased_function_impl() override {}

        std::function<Return(Args...)> func{};
    };

    template <typename T>
    struct __type_erased_this_function_impl;

    template <typename Return, typename... Args>
    struct __type_erased_this_function_impl<Return(Args...)> : __type_erased_function_impl<Return(meta_class*, Args...)>
    {
        ~__type_erased_this_function_impl() override {}

        template <typename T, typename F>
        void set_func(F&& f)
        {
            this->func = std::function<Return(meta_class*, Args...)>(
                [f](meta_class* self, Args... args) -> Return {
                    return std::mem_fn(f)(static_cast<T*>(self), std::forward<Args>(args)...);
                });
        }
    };

    struct property_info
    {
    private:
        std::string m_name;
        std::type_index m_type{ typeid(std::nullptr_t) };
        std::function<std::any(meta_class*)> getter;
        std::function<void(meta_class*, std::any)> setter;

    public:
        std::string_view name() const noexcept { return m_name; }
        std::type_index type() const noexcept { return m_type; }
        bool can_read() const noexcept { return (bool)getter; }
        bool can_write() const noexcept { return (bool)setter; }

        std::any get(meta_class* self) const
        {
            if (getter)
            {
                return getter(self);
            }
            else
            {
                return std::any();
            }
        }
        void set(meta_class* self, std::any value)
        {
            if (setter)
            {
                setter(self, value);
            }
        }

        friend class meta_context;
    };

    struct collection_property_info
    {
    private:
        std::string m_name;
        std::type_index m_type{ typeid(std::nullptr_t) };
        std::function<void(meta_class*, std::any)> adder;
        std::function<void(meta_class*, std::any)> remover;

    public:
        std::string_view name() const noexcept { return m_name; }
        std::type_index type() const noexcept { return m_type; }
        bool can_add() const noexcept { return (bool)adder; }
        bool can_remove() const noexcept { return (bool)remover; }

        void add(meta_class* self, std::any value) const
        {
            if (adder)
            {
                return adder(self, value);
            }
        }
        void remove(meta_class* self, std::any value)
        {
            if (remover)
            {
                remover(self, value);
            }
        }

        friend class meta_context;
    };

    struct event_info
    {
    public:
        using token_type = typename event<>::token_type;

    private:
        std::string m_name;
        std::function<token_type(meta_class*, __type_erased_function*)> adder;
        std::function<token_type(meta_class*, meta_class*, __type_erased_function*)> adder_erased_this;
        std::function<void(meta_class*, token_type)> remover;
        __type_erased_function* invoker;

    public:
        std::string_view name() const noexcept { return m_name; }
        bool can_add() const noexcept { return (bool)adder; }
        bool can_remove() const noexcept { return (bool)remover; }
        bool can_invoke() const noexcept { return (bool)invoker; }

        template <typename... Args>
        bool match_arg_type()
        {
            return invoker && invoker->is_same_arg_type<meta_class*, Args...>();
        }

        template <typename... Args>
        token_type add(meta_class* self, std::function<void(Args...)> handler)
        {
            if (adder)
            {
                if (match_arg_type<Args...>())
                {
                    auto h = __type_erased_function_impl<void(Args...)>{};
                    h.func = handler;
                    return adder(self, &h);
                }
                if constexpr (sizeof...(Args) == 0)
                {
                    auto h = __type_erased_function_impl<void(Args...)>{};
                    h.func = [handler](Args... args) -> void { handler(args...); };
                    return adder(self, &h);
                }
            }
            return 0;
        }

        token_type add_erased_this(meta_class* self, meta_class* target, __type_erased_function* func)
        {
            if (adder_erased_this)
            {
                return adder_erased_this(self, target, func);
            }
            return 0;
        }

        void remove(meta_class* self, token_type token)
        {
            if (remover)
            {
                remover(self, token);
            }
        }

        template <typename... Args>
        void invoke(meta_class* self, Args... args)
        {
            if (match_arg_type<Args...>())
            {
                auto i = static_cast<__type_erased_function_impl<void(meta_class*, Args...)>*>(invoker);
                i->func(self, std::forward<Args>(args)...);
            }
        }

        friend class meta_context;
    };

    enum class binding_mode
    {
        one_time = 0x0,
        one_way = 0x1,
        one_way_to_source = 0x2,
        two_way = one_way | one_way_to_source
    };

    constexpr int operator&(binding_mode lhs, binding_mode rhs) { return (int)lhs & (int)rhs; }
    constexpr int operator|(binding_mode lhs, binding_mode rhs) { return (int)lhs | (int)rhs; }

    class __binding_guard;

    class meta_context
    {
    private:
        std::unordered_map<std::string, std::string> namespace_map;
        std::unordered_map<std::string, std::unordered_map<std::string, std::type_index>> type_map;
        std::unordered_map<std::type_index, std::tuple<std::string, std::string>> type_name_map;
        std::unordered_set<std::type_index> enum_type_set;
        std::unordered_map<std::type_index, std::unordered_map<std::type_index, std::unique_ptr<meta_class>>> attribute_map;
        std::unordered_map<std::type_index, std::unordered_multimap<std::string, std::unique_ptr<__type_erased_function>>> static_method_map;
        std::unordered_multimap<std::type_index, std::unique_ptr<__type_erased_function>> ctor_map;
        std::unordered_map<std::type_index, std::unordered_multimap<std::string, std::unique_ptr<__type_erased_function>>> method_map;
        std::unordered_map<std::type_index, std::unordered_map<std::string, __type_index_wrapper>> prop_type_map;
        std::map<std::size_t, std::unique_ptr<__binding_guard>> bind_map;
        std::size_t bind_index;

    private:
        XAML_META_API std::string get_real_namespace(std::string_view ns);

    public:
        // TYPE METHODS

        // Get type with namespace and name.
        XAML_META_API std::optional<std::type_index> get_type(std::string_view ns, std::string_view name) noexcept;

        // Get the namespace and name of the type.
        XAML_META_API std::optional<std::tuple<std::string, std::string>> get_type_name(std::type_index type) noexcept;

        XAML_META_API void __register_type(std::string_view ns, std::string_view name, std::type_index type) noexcept;

        // Register type with namespace and name.
        template <typename TChild>
        void register_type(std::string_view ns, std::string_view name) noexcept
        {
            __register_type(ns, name, std::type_index(typeid(TChild)));
        }

        XAML_META_API void __register_enum(std::type_index type) noexcept;

        template <typename TEnum, typename = std::enable_if_t<std::is_enum_v<TEnum>>>
        void register_enum(std::string_view ns, std::string_view name) noexcept
        {
            auto t = std::type_index(typeid(TEnum));
            __register_type(ns, name, t);
            __register_enum(t);
        }

        XAML_META_API bool is_registered_enum(std::type_index type) noexcept;

        // Add a map between xmlns and C++ namespace.
        XAML_META_API void add_xml_namespace(std::string_view xmlns, std::string_view ns) noexcept;

        // ATTRIBUTE METHODS

        XAML_META_API meta_class* __get_attribute(std::type_index type, std::type_index attr_type) noexcept;

        // Get an attribute instance with specified type and attribute type.
        template <typename TAttr>
        TAttr* get_attribute(std::type_index type) noexcept
        {
            return static_cast<TAttr*>(__get_attribute(type, std::type_index(typeid(TAttr))));
        }

        // Set an attribute to specified type.
        XAML_META_API void set_attribute(std::type_index type, std::unique_ptr<meta_class>&& attr) noexcept;

        // FUNCTION METHODS

        // FUNCTION METHODS - STATIC METHOD

        XAML_META_API __type_erased_function* __get_static_method(std::type_index type, std::string_view name, std::type_index ret_type, std::initializer_list<std::type_index> arg_types) noexcept;

        template <typename Return, typename... Args>
        std::function<Return(Args...)> get_static_method(std::type_index type, std::string_view name) noexcept
        {
            auto method = __get_static_method(type, name, std::type_index(typeid(Return)), { std::type_index(typeid(Args))... });
            if (method)
            {
                return std::function<Return(Args...)>(static_cast<__type_erased_function_impl<Return(Args...)>*>(method)->func);
            }
            else
            {
                return {};
            }
        }

        template <typename Return, typename... Args>
        __optional_return_t<Return> invoke_static_method(std::type_index type, std::string_view name, Args... args)
        {
            auto m = get_static_method<Return, Args...>(type, name);
            if (m)
            {
                if constexpr (std::is_same_v<Return, void>)
                {
                    m(std::forward<Args>(args)...);
                    return true;
                }
                else
                {
                    return std::make_optional<Return>(m(std::forward<Args>(args)...));
                }
            }
            else
            {
                if constexpr (std::is_same_v<Return, void>)
                {
                    return false;
                }
                else
                {
                    return std::nullopt;
                }
            }
        }

        XAML_META_API void __add_static_method(std::type_index type, std::string_view name, std::unique_ptr<__type_erased_function>&& func) noexcept;

        template <typename T, typename Return, typename... Args>
        void add_static_method_ex(std::string_view name, std::function<Return(Args...)> func)
        {
            if (func)
            {
                auto f = std::make_unique<__type_erased_function_impl<Return(Args...)>>();
                f->func = func;
                __add_static_method(std::type_index(typeid(T)), name, std::move(f));
            }
        }

        template <typename T, typename Return, typename... Args>
        void add_static_method(std::string_view name, Return (*func)(Args...)) noexcept
        {
            if (func)
            {
                add_static_method_ex<T, Return, Args...>(name, std::function<Return(Args...)>(func));
            }
        }

        // FUNCTION METHODS - CONSTRUCTOR

        XAML_META_API __type_erased_function* __get_constructor(std::type_index type, std::initializer_list<std::type_index> arg_types) noexcept;

        template <typename... Args>
        std::function<meta_class*(Args...)> get_constructor(std::type_index type) noexcept
        {
            auto ctor = __get_constructor(type, { std::type_index(typeid(Args))... });
            if (ctor)
            {
                return std::function<meta_class*(Args...)>(
                    static_cast<__type_erased_function_impl<meta_class*(Args...)>*>(ctor)->func);
            }
            else
            {
                return {};
            }
        }

        template <typename... Args>
        meta_class* construct(std::type_index type, Args... args) noexcept
        {
            auto ctor = get_constructor<Args...>(type);
            if (ctor)
            {
                return ctor(std::forward<Args>(args)...);
            }
            else
            {
                return nullptr;
            }
        }

        XAML_META_API void __add_constructor(std::type_index type, std::unique_ptr<__type_erased_function>&& ctor) noexcept;

        template <typename TChild, typename... Args>
        void add_constructor() noexcept
        {
            auto c = std::make_unique<__type_erased_function_impl<meta_class*(Args...)>>();
            c->func = std::function<meta_class*(Args...)>(
                [](Args... args) -> meta_class* {
                    return new TChild(std::forward<Args>(args)...);
                });
            __add_constructor(std::type_index(typeid(TChild)), std::move(c));
        }

        // FUNCTION METHODS - MEMBER METHODS

        XAML_META_API __type_erased_function* __get_method(std::type_index type, std::string_view name, std::type_index ret_type, std::initializer_list<std::type_index> arg_types) noexcept;

        XAML_META_API __type_erased_function* __get_first_method(std::type_index type, std::string_view name) noexcept;

        template <typename Return, typename... Args>
        std::function<Return(meta_class*, Args...)> get_method(std::type_index type, std::string_view name) noexcept
        {
            auto m = __get_method(type, name, std::type_index(typeid(Return)), { std::type_index(typeid(meta_class*)), std::type_index(typeid(Args))... });
            if (m)
            {
                return std::function<Return(meta_class*, Args...)>(
                    static_cast<__type_erased_this_function_impl<Return(Args...)>*>(m)->func);
            }
            else
            {
                return {};
            }
        }

        template <typename Return, typename... Args>
        __optional_return_t<Return> invoke_method(meta_class* obj, std::string_view name, Args... args) noexcept
        {
            auto m = get_method<Return, Args...>(obj->this_type(), name);
            if (m)
            {
                if constexpr (std::is_same_v<Return, void>)
                {
                    m(obj, std::forward<Args>(args)...);
                    return true;
                }
                else
                {
                    return std::make_optional<Return>(m(obj, std::forward<Args>(args)...));
                }
            }
            else
            {
                if constexpr (std::is_same_v<Return, void>)
                {
                    return false;
                }
                else
                {
                    return std::nullopt;
                }
            }
        }

        XAML_META_API void __add_method(std::type_index type, std::string_view name, std::unique_ptr<__type_erased_function>&& func) noexcept;

        template <typename T, typename TBase, typename TMethod>
        void add_method(std::string_view name, TMethod TBase::*func) noexcept
        {
            if (func)
            {
                auto f = std::make_unique<__type_erased_this_function_impl<__remove_const_func_t<TMethod>>>();
                f->template set_func<T>(func);
                __add_method(std::type_index(typeid(T)), name, std::move(f));
            }
        }

        template <typename T, typename Return, typename... Args>
        void add_method_ex(std::string_view name, std::function<Return(meta_class*, Args...)> func)
        {
            if (func)
            {
                auto f = std::make_unique<__type_erased_this_function_impl<__remove_const_func_t<Return(Args...)>>>();
                f->func = func;
                __add_method(std::type_index(typeid(T)), name, std::move(f));
            }
        }

        // PROPERTY

        inline std::string __property_name(std::string_view name)
        {
            return "prop@" + (std::string)name;
        }

        inline std::string __attach_property_name(std::string_view name)
        {
            return "aprop@" + (std::string)name;
        }

        XAML_META_API std::type_index __get_property_type(std::type_index type, std::string_view name) noexcept;

        inline property_info get_property(std::type_index type, std::string_view name)
        {
            property_info info = {};
            info.m_name = name;
            info.m_type = __get_property_type(type, name);
            std::string pname = __property_name(name);
            info.getter = get_method<std::any>(type, pname);
            info.setter = get_method<void, std::any>(type, pname);
            return info;
        }

        inline property_info get_attach_property(std::type_index type, std::string_view name)
        {
            property_info info = {};
            info.m_name = name;
            info.m_type = __get_property_type(type, name);
            std::string pname = __attach_property_name(name);
            info.getter = get_static_method<std::any, meta_class*>(type, pname);
            info.setter = get_static_method<void, meta_class*, std::any>(type, pname);
            return info;
        }

        XAML_META_API void __set_property_type(std::type_index type, std::string_view name, std::type_index prop_type) noexcept;

        template <typename T, typename TChild, typename TValue, typename TGet>
        void add_attach_property_read(std::string_view name, TGet&& getter)
        {
            if (getter)
            {
                std::string pname = __attach_property_name(name);
                add_static_method_ex<T, std::any, meta_class*>(
                    pname,
                    std::function<std::any(meta_class*)>(
                        [getter](meta_class* self) -> std::any {
                            return getter(static_cast<TChild>(*self));
                        }));
            }
        }

        template <typename T, typename TChild, typename TValue, typename TSet>
        void add_attach_property_write(std::string_view name, TSet&& setter)
        {
            if (setter)
            {
                std::string pname = __attach_property_name(name);
                add_static_method_ex<T, void, meta_class*, std::any>(
                    pname,
                    std::function<void(meta_class*, std::any)>(
                        [setter](meta_class* self, std::any value) -> void {
                            setter(static_cast<TChild>(*self), value_converter_traits<TValue>::convert(value));
                        }));
            }
        }

        template <typename T, typename TChild, typename TValue, typename TGet, typename TSet>
        void add_attach_property(std::string_view name, TGet&& getter, TSet&& setter)
        {
            __set_property_type(std::type_index(typeid(T)), name, std::type_index(typeid(TValue)));
            add_attach_property_read<T, TChild, TValue>(name, getter);
            add_attach_property_write<T, TChild, TValue>(name, setter);
        }

        template <typename T, typename TValue>
        void add_property_read_ex(std::string_view name, std::function<TValue(T*)> getter)
        {
            if (getter)
            {
                std::string pname = __property_name(name);
                add_method_ex<T, std::any>(
                    pname,
                    std::function<std::any(meta_class*)>(
                        [getter](meta_class* self) -> std::any {
                            return getter(static_cast<T*>(self));
                        }));
            }
        }

        template <typename T, typename TValue>
        void add_property_write_ex(std::string_view name, std::function<void(T*, TValue)> setter)
        {
            if (setter)
            {
                std::string pname = __property_name(name);
                add_method_ex<T, void, std::any>(
                    pname,
                    std::function<void(meta_class*, std::any)>(
                        [setter](meta_class* self, std::any value) -> void {
                            setter(static_cast<T*>(self), value_converter_traits<TValue>::convert(value));
                        }));
            }
        }

        template <typename T, typename TValue>
        void add_property_ex(std::string_view name, std::function<TValue(T*)> getter, std::function<void(T*, TValue)> setter)
        {
            __set_property_type(std::type_index(typeid(T)), name, std::type_index(typeid(TValue)));
            add_property_read_ex<T, TValue>(name, getter);
            add_property_write_ex<T, TValue>(name, setter);
        }

        // COLLECTION PROPERTY
        // Usually a vector.

        inline std::string __add_collection_property_name(std::string_view name)
        {
            return "cprop@a@" + (std::string)name;
        }

        inline std::string __remove_collection_property_name(std::string_view name)
        {
            return "cprop@r@" + (std::string)name;
        }

        inline std::string __add_attach_collection_property_name(std::string_view name)
        {
            return "caprop@a@" + (std::string)name;
        }

        inline std::string __remove_attach_collection_property_name(std::string_view name)
        {
            return "caprop@r@" + (std::string)name;
        }

        inline collection_property_info get_collection_property(std::type_index type, std::string_view name)
        {
            collection_property_info info = {};
            info.m_name = name;
            info.m_type = __get_property_type(type, name);
            info.adder = get_method<void, std::any>(type, __add_collection_property_name(name));
            info.remover = get_method<void, std::any>(type, __remove_collection_property_name(name));
            return info;
        }

        inline collection_property_info get_attach_collection_property(std::type_index type, std::string_view name)
        {
            collection_property_info info = {};
            info.m_name = name;
            info.m_type = __get_property_type(type, name);
            info.adder = get_static_method<void, meta_class*, std::any>(type, __add_attach_collection_property_name(name));
            info.remover = get_static_method<void, meta_class*, std::any>(type, __remove_attach_collection_property_name(name));
            return info;
        }

        template <typename T, typename TValue>
        void add_collection_property_add(std::string_view name, std::function<void(T*, TValue)> adder)
        {
            if (adder)
            {
                std::string pname = __add_collection_property_name(name);
                add_method_ex<T, void, std::any>(
                    pname,
                    std::function<void(meta_class*, std::any)>(
                        [adder](meta_class* self, std::any value) -> void {
                            return adder(static_cast<T*>(self), value_converter_traits<TValue>::convert(value));
                        }));
            }
        }

        template <typename T, typename TValue>
        void add_collection_property_remove(std::string_view name, std::function<void(T*, TValue)> remover)
        {
            if (remover)
            {
                std::string pname = __remove_collection_property_name(name);
                add_method_ex<T, void, std::any>(
                    pname,
                    std::function<void(meta_class*, std::any)>(
                        [remover](meta_class* self, std::any value) -> void {
                            remover(static_cast<T*>(self), value_converter_traits<TValue>::convert(value));
                        }));
            }
        }

        template <typename T, typename TValue>
        void add_collection_property(std::string_view name, std::function<void(T*, TValue)> adder, std::function<void(T*, TValue)> remover)
        {
            add_collection_property_add<T, TValue>(name, adder);
            add_collection_property_remove<T, TValue>(name, remover);
            __set_property_type(std::type_index(typeid(T)), name, std::type_index(typeid(TValue)));
        }

        template <typename T, typename TChild, typename TValue, typename TAdd>
        void add_attach_collection_property_add(std::string_view name, TAdd&& adder)
        {
            if (adder)
            {
                std::string pname = __add_attach_collection_property_name(name);
                add_static_method_ex<T, void, meta_class*>(
                    pname,
                    std::function<void(meta_class*, std::any)>(
                        [adder](meta_class* self, std::any value) -> void {
                            return adder(static_cast<TChild>(*self), value_converter_traits<TValue>::convert(value));
                        }));
            }
        }

        template <typename T, typename TChild, typename TValue, typename TRemove>
        void add_attach_collection_property_remove(std::string_view name, TRemove&& remover)
        {
            if (remover)
            {
                std::string pname = __remove_attach_collection_property_name(name);
                add_static_method_ex<T, void, meta_class*, std::any>(
                    pname,
                    std::function<void(meta_class*, std::any)>(
                        [remover](meta_class* self, std::any value) -> void {
                            remover(static_cast<TChild>(*self), value_converter_traits<TValue>::convert(value));
                        }));
            }
        }

        template <typename T, typename TChild, typename TValue, typename TAdd, typename TRemove>
        void add_attach_collection_property(std::string_view name, TAdd&& adder, TRemove&& remover)
        {
            add_attach_collection_property_add<T, TChild, TValue>(name, adder);
            add_attach_collection_property_remove<T, TChild, TValue>(name, remover);
            __set_property_type(std::type_index(typeid(T)), name, std::type_index(typeid(TValue)));
        }

        // EVENT

        inline std::string __event_name(std::string_view name)
        {
            return "event@" + (std::string)name;
        }

        inline event_info __get_event(std::type_index type, std::string_view name, std::initializer_list<std::type_index> arg_types)
        {
            event_info info = {};
            info.m_name = name;
            std::string ename = __event_name(name);
            info.adder = get_method<typename event_info::token_type, __type_erased_function*>(type, ename);
            info.adder_erased_this = get_method<typename event_info::token_type, meta_class*, __type_erased_function*>(type, ename);
            info.remover = get_method<void, typename event_info::token_type>(type, ename);
            info.invoker = __get_method(type, ename, std::type_index(typeid(void)), arg_types);
            return info;
        }

        template <typename... Args>
        event_info get_event(std::type_index type, std::string_view name)
        {
            return __get_event(type, name, { std::type_index(typeid(meta_class*)), std::type_index(typeid(Args))... });
        }

        template <typename T, typename... Args>
        void add_event_add_ex(std::string_view name, std::function<typename event_info::token_type(T*, std::function<void(Args...)>)> adder)
        {
            if (adder)
            {
                std::string ename = __event_name(name);
                add_method_ex<T, typename event_info::token_type, __type_erased_function*>(
                    ename,
                    std::function<typename event_info::token_type(meta_class*, __type_erased_function*)>(
                        [adder](meta_class * self, __type_erased_function * handler) -> typename event_info::token_type {
                            auto h = static_cast<__type_erased_function_impl<void(Args...)>*>(handler);
                            return adder(static_cast<T*>(self), std::move(h->func));
                        }));
            }
        }

        template <typename T, typename... Args>
        void add_event_add_erased_this_ex(std::string_view name, std::function<typename event_info::token_type(T*, std::function<void(Args...)>)> adder)
        {
            if (adder)
            {
                std::string ename = __event_name(name);
                add_method_ex<T, typename event_info::token_type, meta_class*, __type_erased_function*>(
                    ename,
                    std::function<typename event_info::token_type(meta_class*, meta_class*, __type_erased_function*)>(
                        [adder](meta_class * self, meta_class * target, __type_erased_function * handler) -> typename event_info::token_type {
                            auto h = static_cast<__type_erased_this_function_impl<void(Args...)>*>(handler);
                            auto func = h->func;
                            return adder(static_cast<T*>(self), [target, func](Args... args) { func(target, std::forward<Args>(args)...); });
                        }));
            }
        }

        template <typename T, typename... Args>
        void add_event_remove_ex(std::string_view name, std::function<void(T*, typename event_info::token_type)> remover)
        {
            if (remover)
            {
                std::string ename = __event_name(name);
                add_method_ex<T, void, typename event_info::token_type>(
                    ename,
                    std::function<void(meta_class*, typename event_info::token_type)>(
                        [remover](meta_class* self, typename event_info::token_type token) -> void {
                            remover(static_cast<T*>(self), token);
                        }));
            }
        }

        template <typename T, typename... Args>
        void add_event_invoke_ex(std::string_view name, std::function<event<Args...>&(T*)> getter)
        {
            if (getter)
            {
                std::string ename = __event_name(name);
                add_method_ex<T, void, Args...>(
                    ename,
                    std::function<void(meta_class*, Args...)>(
                        [getter](meta_class* self, Args... args) -> void {
                            getter(static_cast<T*>(self))(std::forward<Args>(args)...);
                        }));
            }
        }

        template <typename T, typename... Args>
        void add_event_ex(std::string_view name, std::function<typename event_info::token_type(T*, std::function<void(Args...)>)> adder, std::function<void(T*, typename event_info::token_type)> remover, std::function<event<Args...>&(T*)> getter)
        {
            add_event_add_ex<T, Args...>(name, adder);
            add_event_add_erased_this_ex<T, Args...>(name, adder);
            add_event_remove_ex<T, Args...>(name, remover);
            add_event_invoke_ex<T, Args...>(name, getter);
        }

        XAML_META_API std::size_t bind(std::weak_ptr<meta_class> target, std::string_view target_prop, std::weak_ptr<meta_class> source, std::string_view source_prop, binding_mode mode = binding_mode::one_time);
        XAML_META_API void unbind(std::size_t token);
    };

    inline std::string __get_property_changed_event_name(std::string_view name)
    {
        return (std::string)name + "_changed";
    }

    class __binding_guard
    {
    private:
        using token_type = typename event<>::token_type;

        std::weak_ptr<meta_class> target;
        std::weak_ptr<meta_class> source;

        property_info target_prop;
        event_info target_event;
        property_info source_prop;
        event_info source_event;

        token_type target_token;
        token_type source_token;

    public:
        __binding_guard(meta_context& ctx, std::weak_ptr<meta_class> target, std::string_view target_prop, std::weak_ptr<meta_class> source, std::string_view source_prop, binding_mode mode = binding_mode::one_time)
            : target(target), source(source)
        {
            auto starget = target.lock();
            auto ssource = source.lock();
            this->target_prop = ctx.get_property(target.lock()->this_type(), target_prop);
            target_event = ctx.get_event(target.lock()->this_type(), __get_property_changed_event_name(target_prop));
            this->source_prop = ctx.get_property(source.lock()->this_type(), source_prop);
            source_event = ctx.get_event(source.lock()->this_type(), __get_property_changed_event_name(source_prop));
            this->target_prop.set(starget.get(), this->source_prop.get(ssource.get()));
            if (mode & binding_mode::one_way)
            {
                source_token = source_event.add(
                    ssource.get(),
                    std::function<void()>(
                        [this]() -> void {
                            if (auto ssource = this->source.lock())
                                if (auto starget = this->target.lock())
                                    this->target_prop.set(starget.get(), this->source_prop.get(ssource.get()));
                        }));
            }
            if (mode & binding_mode::one_way_to_source)
            {
                target_token = target_event.add(
                    starget.get(),
                    std::function<void()>(
                        [this]() -> void {
                            if (auto ssource = this->source.lock())
                                if (auto starget = this->target.lock())
                                    this->source_prop.set(ssource.get(), this->target_prop.get(starget.get()));
                        }));
            }
        }

        ~__binding_guard()
        {
            if (auto ssource = source.lock())
                source_event.remove(ssource.get(), source_token);
            if (auto starget = target.lock())
                target_event.remove(starget.get(), target_token);
        }
    };

    // REGISTER CLASS METHOD

    template <typename... T>
    struct __register_class_helper;

    template <typename T1, typename... Ts>
    struct __register_class_helper<T1, Ts...>
    {
        void operator()(meta_context& ctx) noexcept(noexcept(T1::register_class(ctx)) && noexcept(__register_class_helper<Ts...>{}(ctx)))
        {
            T1::register_class(ctx);
            __register_class_helper<Ts...>{}(ctx);
        }
    };

    template <>
    struct __register_class_helper<>
    {
        void operator()(meta_context& ctx) noexcept {}
    };

    // Register all classes from the type parameters.
    template <typename... T>
    void register_class(meta_context& ctx) noexcept(noexcept(__register_class_helper<T...>{}(ctx)))
    {
        __register_class_helper<T...>{}(ctx);
    }

    template <typename T, typename TEvent>
    struct __add_event_deduce_helper;

    template <typename T, typename... Args>
    struct __add_event_deduce_helper<T, event<Args...>&>
    {
        void operator()(meta_context& ctx, std::string_view name, std::function<typename event_info::token_type(T*, std::function<void(Args...)>)> adder, std::function<void(T*, typename event_info::token_type)> remover, std::function<event<Args...>&(T*)> getter) const
        {
            ctx.add_event_ex<T, Args...>(name, std::move(adder), std::move(remover), std::move(getter));
        }
    };
} // namespace xaml

#endif // !XAML_META_HPP
