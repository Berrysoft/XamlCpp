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
#include <xaml/array_view.hpp>
#include <xaml/meta/conv.hpp>
#include <xaml/meta/event.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/utility.hpp>

namespace xaml
{
    // BASE TYPE

    // Base class of all classes which is registered for reflection.
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
    struct value_converter_traits<T&, std::enable_if_t<std::is_base_of_v<meta_class, T>>>
    {
        static T& convert(std::any value)
        {
            if (value.type() == typeid(T&))
            {
                return std::any_cast<T&>(value);
            }
            else if (value.type() == typeid(meta_class&))
            {
                return static_cast<T&>(std::any_cast<meta_class&>(value));
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
        void set(meta_class* self, std::any value) const
        {
            if (setter)
            {
                setter(self, value);
            }
        }

        friend class reflection_info;
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
        void remove(meta_class* self, std::any value) const
        {
            if (remover)
            {
                remover(self, value);
            }
        }

        friend class reflection_info;
    };

    struct event_info
    {
    public:
        using token_type = typename event<>::token_type;

    private:
        std::string m_name;
        std::function<token_type(meta_class*, __type_erased_function const*)> adder;
        std::function<token_type(meta_class*, meta_class*, __type_erased_function const*)> adder_erased_this;
        std::function<void(meta_class*, token_type)> remover;
        std::unique_ptr<__type_erased_function> invoker;

    public:
        std::string_view name() const noexcept { return m_name; }
        bool can_add() const noexcept { return (bool)adder; }
        bool can_remove() const noexcept { return (bool)remover; }
        bool can_invoke() const noexcept { return (bool)invoker; }

        template <typename... Args>
        bool match_arg_type() const
        {
            return invoker && invoker->is_same_arg_type<meta_class*, Args...>();
        }

        template <typename... Args>
        token_type add(meta_class* self, std::function<void(Args...)> handler) const
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

        token_type add_erased_this(meta_class* self, meta_class* target, __type_erased_function const* func) const
        {
            if (adder_erased_this)
            {
                return adder_erased_this(self, target, func);
            }
            return 0;
        }

        void remove(meta_class* self, token_type token) const
        {
            if (remover)
            {
                remover(self, token);
            }
        }

        template <typename... Args>
        void invoke(meta_class* self, Args... args) const
        {
            if (match_arg_type<Args...>())
            {
                auto i = static_cast<__type_erased_function_impl<void(meta_class*, Args...)> const*>(invoker.get());
                i->func(self, std::forward<Args>(args)...);
            }
        }

        friend class reflection_info;
    };

    class reflection_info
    {
    private:
        std::type_index m_type{ typeid(std::nullptr_t) };
        std::tuple<std::string, std::string> m_name;
        std::unordered_map<std::type_index, std::unique_ptr<meta_class>> m_attribute_map;
        std::unordered_multimap<std::string, std::unique_ptr<__type_erased_function>> m_static_method_map;
        std::vector<std::unique_ptr<__type_erased_function>> m_ctors;
        std::unordered_multimap<std::string, std::unique_ptr<__type_erased_function>> m_method_map;
        std::unordered_map<std::string, std::unique_ptr<property_info>> m_prop_map;
        std::unordered_map<std::string, std::unique_ptr<collection_property_info>> m_collection_prop_map;
        std::unordered_map<std::string, std::unique_ptr<event_info>> m_event_map;

    public:
        std::type_index get_type() const noexcept { return m_type; }
        std::tuple<std::string, std::string> const& get_type_name() const noexcept { return m_name; }

    public:
        std::unordered_map<std::type_index, std::unique_ptr<meta_class>> const& get_attributes() const noexcept { return m_attribute_map; }

    protected:
        XAML_META_API meta_class const* __get_attribute(std::type_index attr_type) const noexcept;

    public:
        template <typename TAttr>
        TAttr const* get_attribute() const noexcept
        {
            return static_cast<TAttr const*>(__get_attribute(std::type_index{ typeid(TAttr) }));
        }

        XAML_META_API void set_attribute(std::unique_ptr<meta_class>&& attr) noexcept;

    public:
        std::unordered_multimap<std::string, std::unique_ptr<__type_erased_function>> const& get_static_methods() const noexcept { return m_static_method_map; }

    protected:
        XAML_META_API __type_erased_function const* __get_static_method(std::string_view name, std::type_index ret_type, std::initializer_list<std::type_index> arg_types) const noexcept;

    public:
        template <typename Return, typename... Args>
        std::function<Return(Args...)> get_static_method(std::string_view name) const noexcept
        {
            auto method = __get_static_method(name, std::type_index(typeid(Return)), { std::type_index(typeid(Args))... });
            if (method)
            {
                return static_cast<__type_erased_function_impl<Return(Args...)> const*>(method)->func;
            }
            else
            {
                return {};
            }
        }

        template <typename Return, typename... Args>
        __optional_return_t<Return> invoke_static_method(std::string_view name, Args... args) const
        {
            auto m = get_static_method<Return, Args...>(name);
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

    protected:
        XAML_META_API void __add_static_method(std::string_view name, std::unique_ptr<__type_erased_function>&& func) noexcept;

    public:
        template <typename Return, typename... Args>
        void add_static_method(std::string_view name, std::function<Return(Args...)>&& func)
        {
            if (func)
            {
                auto f = std::make_unique<__type_erased_function_impl<Return(Args...)>>();
                f->func = func;
                __add_static_method(name, std::move(f));
            }
        }

    public:
        array_view<std::unique_ptr<__type_erased_function>> get_constructors() const noexcept { return m_ctors; }

    protected:
        XAML_META_API __type_erased_function const* __get_constructor(std::initializer_list<std::type_index> arg_types) const noexcept;

    public:
        template <typename... Args>
        std::function<meta_class*(Args...)> get_constructor() const noexcept
        {
            auto ctor = __get_constructor({ std::type_index(typeid(Args))... });
            if (ctor)
            {
                return static_cast<__type_erased_function_impl<meta_class*(Args...)> const*>(ctor)->func;
            }
            else
            {
                return {};
            }
        }

        template <typename... Args>
        meta_class* construct(Args... args) const noexcept
        {
            auto ctor = get_constructor<Args...>();
            if (ctor)
            {
                return ctor(std::forward<Args>(args)...);
            }
            else
            {
                return nullptr;
            }
        }

    protected:
        XAML_META_API void __add_constructor(std::unique_ptr<__type_erased_function>&& ctor) noexcept;

    public:
        template <typename T, typename... Args>
        void add_constructor() noexcept
        {
            auto c = std::make_unique<__type_erased_function_impl<meta_class*(Args...)>>();
            c->func = [](Args... args) -> meta_class* { return new T(std::forward<Args>(args)...); };
            __add_constructor(std::move(c));
        }

    public:
        std::unordered_multimap<std::string, std::unique_ptr<__type_erased_function>> const& get_methods() const noexcept { return m_method_map; }

    protected:
        XAML_META_API __type_erased_function const* __get_method(std::string_view name, std::type_index ret_type, std::initializer_list<std::type_index> arg_types) const noexcept;

    public:
        template <typename Return, typename... Args>
        std::function<Return(meta_class*, Args...)> get_method(std::string_view name) const noexcept
        {
            auto m = __get_method(name, std::type_index(typeid(Return)), { std::type_index(typeid(meta_class*)), std::type_index(typeid(Args))... });
            if (m)
            {
                return static_cast<__type_erased_this_function_impl<Return(Args...)> const*>(m)->func;
            }
            else
            {
                return {};
            }
        }

        template <typename Return, typename... Args>
        __optional_return_t<Return> invoke_method(meta_class& obj, std::string_view name, Args... args) const noexcept
        {
            auto m = get_method<Return, Args...>(name);
            if (m)
            {
                if constexpr (std::is_same_v<Return, void>)
                {
                    m(&obj, std::forward<Args>(args)...);
                    return true;
                }
                else
                {
                    return std::make_optional<Return>(m(&obj, std::forward<Args>(args)...));
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

    protected:
        XAML_META_API void __add_method(std::string_view name, std::unique_ptr<__type_erased_function>&& func) noexcept;

    public:
        template <typename Return, typename... Args>
        void add_method(std::string_view name, std::function<Return(meta_class*, Args...)>&& func)
        {
            if (func)
            {
                auto f = std::make_unique<__type_erased_this_function_impl<__remove_const_func_t<Return(Args...)>>>();
                f->func = std::move(func);
                __add_method(name, std::move(f));
            }
        }

    public:
        std::unordered_map<std::string, std::unique_ptr<property_info>> const& get_properties() const noexcept { return m_prop_map; }

        XAML_META_API property_info const* get_property(std::string_view name) const noexcept;

    protected:
        XAML_META_API void __add_property(std::string_view name, std::type_index type, std::function<std::any(meta_class*)>&& getter, std::function<void(meta_class*, std::any)>&& setter, bool attach);

    public:
        template <typename T>
        void add_property(std::string_view name, std::function<std::any(meta_class*)>&& getter, std::function<void(meta_class*, std::any)>&& setter, bool attach = false)
        {
            __add_property(name, std::type_index(typeid(T)), std::move(getter), std::move(setter), attach);
        }

    public:
        std::unordered_map<std::string, std::unique_ptr<collection_property_info>> const& get_collection_properties() const noexcept { return m_collection_prop_map; }

        XAML_META_API collection_property_info const* get_collection_property(std::string_view name) const noexcept;

    protected:
        XAML_META_API void __add_collection_property(std::string_view name, std::type_index type, std::function<void(meta_class*, std::any)>&& adder, std::function<void(meta_class*, std::any)>&& remover, bool attach);

    public:
        template <typename T>
        void add_collection_property(std::string_view name, std::function<void(meta_class*, std::any)>&& adder, std::function<void(meta_class*, std::any)>&& remover, bool attach = false)
        {
            __add_collection_property(name, std::type_index(typeid(T)), std::move(adder), std::move(remover), attach);
        }

    public:
        XAML_META_API event_info const* get_event(std::string_view name) const noexcept;

    protected:
        XAML_META_API void __add_event(std::string_view name, std::function<std::size_t(meta_class*, __type_erased_function const*)>&& adder, std::function<std::size_t(meta_class*, meta_class*, __type_erased_function const*)>&& adder_erased_this, std::function<void(meta_class*, std::size_t)>&& remover, std::unique_ptr<__type_erased_function>&& invoker);

    public:
        template <typename... Args>
        void add_event(std::string_view name, std::function<std::size_t(meta_class*, __type_erased_function const*)>&& adder, std::function<void(meta_class*, std::size_t)>&& remover, std::function<void(meta_class*, Args...)>&& invoker)
        {
            if (invoker)
            {
                auto i = std::make_unique<__type_erased_function_impl<void(meta_class*, Args...)>>();
                i->func = std::move(invoker);
                __add_event(
                    name, std::move(adder),
                    [adder](meta_class* self, meta_class* target, __type_erased_function const* func) -> std::size_t {
                        auto f = static_cast<__type_erased_this_function_impl<void(Args...)> const*>(func);
                        __type_erased_function_impl<void(Args...)> h{};
                        h.func = [target, f](Args... args) { f->func(target, std::forward<Args>(args)...); };
                        return adder(self, &h);
                    },
                    std::move(remover), std::move(i));
            }
        }

        template <typename T>
        friend std::unique_ptr<reflection_info> __make_reflection_info(std::string_view ns, std::string_view name);
    };

    template <typename T>
    std::unique_ptr<reflection_info> __make_reflection_info(std::string_view ns, std::string_view name)
    {
        auto ref = std::make_unique<reflection_info>();
        ref->m_type = std::type_index(typeid(T));
        ref->m_name = std::make_tuple<std::string, std::string>((std::string)ns, (std::string)name);
        return ref;
    }

    template <typename T, typename TMethod>
    struct __add_method_deduce_helper;

    template <typename T, typename Return, typename... Args>
    struct __add_method_deduce_helper<T, Return (T::*)(Args...)>
    {
        void operator()(reflection_info& ref, std::string_view name, Return (T::*method)(Args...))
        {
            ref.add_method<Return, Args...>(name, std::function<Return(meta_class*, Args...)>([method](meta_class* self, Args... args) -> Return { return (((T*)self)->*method)(std::forward<Args>(args)...); }));
        }
    };

    template <typename T, typename TMethod>
    struct __add_static_method_deduce_helper;

    template <typename T, typename Return, typename... Args>
    struct __add_static_method_deduce_helper<T, Return (*)(Args...)>
    {
        void operator()(reflection_info& ref, std::string_view name, Return (*method)(Args...))
        {
            ref.add_static_method<Return, Args...>(name, std::function<Return(Args...)>(method));
        }
    };

    template <typename T, typename TEvent>
    struct __add_event_deduce_helper;

    template <typename T, typename... Args>
    struct __add_event_deduce_helper<T, event<Args...>>
    {
        void operator()(reflection_info& ref, std::string_view name, std::function<std::size_t(meta_class*, std::function<void(Args...)>)>&& adder, std::function<void(meta_class*, std::size_t)>&& remover, event<Args...> T::*getter) const
        {
            ref.add_event<Args...>(
                name,
                std::function<std::size_t(meta_class*, __type_erased_function const*)>([adder](meta_class* self, __type_erased_function const* f) -> std::size_t { return adder(self, static_cast<__type_erased_function_impl<void(Args...)> const*>(f)->func); }),
                std::move(remover),
                std::function<void(meta_class*, Args...)>([getter](meta_class* self, Args... args) { (((T*)self)->*getter)(std::forward<Args>(args)...); }));
        }
    };

    class enum_reflection_info
    {
    private:
        std::type_index m_type{ typeid(std::nullptr_t) };
        std::tuple<std::string, std::string> m_name;

    public:
        std::type_index get_type() const noexcept { return m_type; }
        std::tuple<std::string, std::string> const& get_type_name() const noexcept { return m_name; }

        template <typename T>
        friend std::unique_ptr<enum_reflection_info> __make_enum_reflection_info(std::string_view ns, std::string_view name);
    };

    template <typename T>
    std::unique_ptr<enum_reflection_info> __make_enum_reflection_info(std::string_view ns, std::string_view name)
    {
        auto ref = std::make_unique<enum_reflection_info>();
        ref->m_type = std::type_index(typeid(T));
        ref->m_name = std::make_tuple<std::string, std::string>((std::string)ns, (std::string)name);
        return ref;
    }

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
        std::unordered_map<std::string_view, std::unique_ptr<module>> modules_map;

        std::unordered_map<std::string, std::string> namespace_map;
        std::unordered_map<std::string, std::unordered_map<std::string, std::type_index>> type_map;
        std::unordered_map<std::type_index, std::unique_ptr<enum_reflection_info>> enum_map;
        std::unordered_map<std::type_index, std::unique_ptr<reflection_info>> ref_map;
        std::map<std::size_t, std::unique_ptr<__binding_guard>> bind_map;
        std::size_t bind_index{ 0 };

    public:
        XAML_META_API module* add_module(std::string_view path);
        std::unordered_map<std::string_view, std::unique_ptr<module>> const& get_modules() const { return modules_map; }

        XAML_META_API std::string get_real_namespace(std::string_view ns) const;

        // Add a map between xmlns and C++ namespace.
        XAML_META_API void add_xml_namespace(std::string_view xmlns, std::string_view ns) noexcept;

        // Get type with namespace and name.
        XAML_META_API reflection_info const* get_type(std::string_view ns, std::string_view name) const noexcept;

        XAML_META_API reflection_info const* get_type(std::type_index type) const noexcept;

        XAML_META_API void register_type(std::unique_ptr<reflection_info>&& ref) noexcept;

        XAML_META_API enum_reflection_info const* get_enum_type(std::type_index type) const noexcept;

        XAML_META_API void register_type(std::unique_ptr<enum_reflection_info>&& ref) noexcept;

        XAML_META_API bool is_registered_enum(std::type_index type) const noexcept;

        XAML_META_API std::size_t bind(std::weak_ptr<meta_class> target, std::string_view target_prop, std::weak_ptr<meta_class> source, std::string_view source_prop, binding_mode mode = binding_mode::one_time);
        XAML_META_API void unbind(std::size_t token);
    };

    class __binding_guard
    {
    private:
        using token_type = typename event<>::token_type;

        std::weak_ptr<meta_class> target;
        std::weak_ptr<meta_class> source;

        property_info const* target_prop;
        event_info const* target_event;
        property_info const* source_prop;
        event_info const* source_event;

        std::optional<token_type> target_token;
        std::optional<token_type> source_token;

    public:
        XAML_META_API __binding_guard(meta_context& ctx, std::weak_ptr<meta_class> target, std::string_view target_prop, std::weak_ptr<meta_class> source, std::string_view source_prop, binding_mode mode = binding_mode::one_time);
        XAML_META_API ~__binding_guard();
    };

    // REGISTER CLASS METHOD

    template <typename... T>
    struct __register_class_helper;

    template <typename T1, typename... Ts>
    struct __register_class_helper<T1, Ts...>
    {
        void operator()(meta_context& ctx) noexcept(noexcept(T1::register_class()) && noexcept(__register_class_helper<Ts...>{}(ctx)))
        {
            std::unique_ptr<reflection_info> ref = T1::register_class();
            ctx.register_type(std::move(ref));
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
} // namespace xaml

#endif // !XAML_META_HPP
