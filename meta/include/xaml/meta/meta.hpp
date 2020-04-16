#ifndef XAML_META_HPP
#define XAML_META_HPP

#include <algorithm>
#include <filesystem>
#include <functional>
#include <initializer_list>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <xaml/array_view.hpp>
#include <xaml/meta/event.hpp>
#include <xaml/meta/guid.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/utility.hpp>

namespace xaml
{
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
        guid m_type{};
        std::function<std::shared_ptr<meta_class>(std::shared_ptr<meta_class>)> getter;
        std::function<void(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>)> setter;

    public:
        std::string_view name() const noexcept { return m_name; }
        guid type() const noexcept { return m_type; }
        bool can_read() const noexcept { return (bool)getter; }
        bool can_write() const noexcept { return (bool)setter; }

        std::shared_ptr<meta_class> get(std::shared_ptr<meta_class> self) const
        {
            if (getter)
            {
                return getter(self);
            }
            else
            {
                return std::shared_ptr<meta_class>();
            }
        }
        void set(std::shared_ptr<meta_class> self, std::shared_ptr<meta_class> value) const
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
        guid m_type{};
        std::function<void(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>)> adder;
        std::function<void(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>)> remover;

    public:
        std::string_view name() const noexcept { return m_name; }
        guid type() const noexcept { return m_type; }
        bool can_add() const noexcept { return (bool)adder; }
        bool can_remove() const noexcept { return (bool)remover; }

        void add(std::shared_ptr<meta_class> self, std::shared_ptr<meta_class> value) const
        {
            if (adder)
            {
                return adder(self, value);
            }
        }
        void remove(std::shared_ptr<meta_class> self, std::shared_ptr<meta_class> value) const
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
        std::vector<guid> args_type;
        std::function<token_type(std::shared_ptr<meta_class>, type_erased_function const&)> adder;
        std::function<token_type(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>, type_erased_this_function const&)> adder_this;
        std::function<void(std::shared_ptr<meta_class>, token_type)> remover;
        std::unique_ptr<type_erased_this_function> invoker;

    public:
        std::string_view name() const noexcept { return m_name; }
        bool can_add() const noexcept { return (bool)adder; }
        bool can_remove() const noexcept { return (bool)remover; }
        bool can_invoke() const noexcept { return (bool)invoker; }

        constexpr array_view<guid> get_args_type() const noexcept { return args_type; }

        bool is_same_arg_type(std::initializer_list<guid> ts) const noexcept
        {
            return std::equal(args_type.begin(), args_type.end(), ts.begin(), ts.end());
        }

        template <typename... Args>
        bool is_same_arg_type() const noexcept
        {
            return is_same_arg_type({ type_guid_v<Args>... });
        }

        token_type add(std::shared_ptr<meta_class> self, type_erased_function const& handler) const
        {
            if (adder)
            {
                return adder(self, handler);
            }
            return 0;
        }

        token_type add_this(std::shared_ptr<meta_class> self, std::shared_ptr<meta_class> target, type_erased_this_function const& func) const
        {
            if (adder_this)
            {
                return adder_this(self, target, func);
            }
            return 0;
        }

        void remove(std::shared_ptr<meta_class> self, token_type token) const
        {
            if (remover)
            {
                remover(self, token);
            }
        }

        template <typename... Args>
        void invoke(std::shared_ptr<meta_class> self, Args... args) const
        {
            invoker(self, { box_value(std::forward<Args>(args))... });
        }

        friend class reflection_info;
    };

    class reflection_info_base
    {
    protected:
        guid m_type{};
        std::tuple<std::string, std::string> m_name;
        std::string m_include;

    public:
        guid get_type() const noexcept { return m_type; }
        std::tuple<std::string, std::string> const& get_type_name() const noexcept { return m_name; }
        std::string_view get_include_file() const noexcept { return m_include; }

        reflection_info_base(guid type, std::string_view ns, std::string_view name, std::string_view include)
            : m_type(type), m_name(std::make_tuple<std::string, std::string>((std::string)ns, (std::string)name)), m_include(include)
        {
        }
        virtual ~reflection_info_base() {}
    };

    class reflection_info : public reflection_info_base
    {
    private:
        std::unordered_map<guid, std::shared_ptr<meta_class>> m_attribute_map;
        std::vector<std::unique_ptr<type_erased_function>> m_ctors;
        std::unordered_multimap<std::string, std::unique_ptr<type_erased_this_function>> m_method_map;
        std::unordered_multimap<std::string, std::unique_ptr<type_erased_function>> m_static_method_map;
        std::unordered_map<std::string, std::unique_ptr<property_info>> m_prop_map;
        std::unordered_map<std::string, std::unique_ptr<collection_property_info>> m_collection_prop_map;
        std::unordered_map<std::string, std::unique_ptr<event_info>> m_event_map;

    public:
        using reflection_info_base::reflection_info_base;
        ~reflection_info() override {}

    public:
        std::unordered_map<guid, std::shared_ptr<meta_class>> const& get_attributes() const noexcept { return m_attribute_map; }

    protected:
        XAML_META_API std::shared_ptr<meta_class> __get_attribute(guid attr_type) const noexcept;

    public:
        template <typename TAttr>
        std::shared_ptr<TAttr> get_attribute() const noexcept
        {
            return std::static_pointer_cast<TAttr>(__get_attribute(type_guid_v<TAttr>));
        }

        XAML_META_API void set_attribute(std::shared_ptr<meta_class> attr) noexcept;

    public:
        auto const& get_static_methods() const noexcept { return m_static_method_map; }

    protected:
        XAML_META_API type_erased_function const& __get_static_method(std::string_view name, guid ret_type, std::initializer_list<guid> arg_types) const noexcept;

    public:
        template <typename Return, typename... Args>
        __optional_return_t<Return> invoke_static_method(std::string_view name, Args... args) const
        {
            auto& m = __get_static_method(name, type_guid_v<box_value_t<Return>>, { type_guid_v<box_value_t<Args>>... });
            if (m)
            {
                if constexpr (std::is_same_v<Return, void>)
                {
                    unbox_value<Return>(m({ std::forward<Args>(args)... }));
                    return true;
                }
                else
                {
                    return std::make_optional<Return>(unbox_value<Return>(m({ box_value(std::forward<Args>(args))... })));
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

    public:
        XAML_META_API void add_static_method(std::string_view name, std::unique_ptr<type_erased_function>&& func) noexcept;

    public:
        array_view<std::unique_ptr<type_erased_function>> get_constructors() const noexcept { return m_ctors; }

    protected:
        XAML_META_API type_erased_function const& __get_constructor(std::initializer_list<guid> arg_types) const noexcept;

    public:
        template <typename... Args>
        std::shared_ptr<meta_class> construct(Args... args) const noexcept
        {
            auto& ctor = __get_constructor({ type_guid_v<Args>... });
            if (ctor)
            {
                return ctor({ std::forward<Args>(args)... });
            }
            else
            {
                return nullptr;
            }
        }

    public:
        XAML_META_API void add_constructor(std::unique_ptr<type_erased_function>&& ctor) noexcept;

        template <typename T, typename... Args>
        void add_constructor() noexcept
        {
            add_constructor(make_type_erased_function<std::shared_ptr<meta_class>, Args...>([](Args... args) -> std::shared_ptr<meta_class> { return std::make_shared<T>(std::forward<Args>(args)...); }));
        }

    public:
        auto const& get_methods() const noexcept { return m_method_map; }

    protected:
        XAML_META_API type_erased_this_function const& __get_method(std::string_view name, guid ret_type, std::initializer_list<guid> arg_types) const noexcept;

    public:
        template <typename Return, typename... Args>
        __optional_return_t<Return> invoke_method(std::shared_ptr<meta_class> obj, std::string_view name, Args... args) const noexcept
        {
            auto& m = __get_method(name, type_guid_v<box_value_t<Return>>, { type_guid_v<box_value_t<Args>>... });
            if (m)
            {
                if constexpr (std::is_same_v<Return, void>)
                {
                    m(obj, { box_value(std::forward<Args>(args))... });
                    return true;
                }
                else
                {
                    return std::make_optional<Return>(unbox_value<Return>(m(&obj, { box_value(std::forward<Args>(args))... })));
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

    public:
        XAML_META_API void add_method(std::string_view name, std::unique_ptr<type_erased_this_function>&& func) noexcept;

    public:
        std::unordered_map<std::string, std::unique_ptr<property_info>> const& get_properties() const noexcept { return m_prop_map; }

        XAML_META_API property_info const* get_property(std::string_view name) const noexcept;

    protected:
        XAML_META_API void __add_property(std::string_view name, guid type, std::function<std::shared_ptr<meta_class>(std::shared_ptr<meta_class>)>&& getter, std::function<void(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>)>&& setter, bool attach);

    public:
        template <typename T>
        void add_property(std::string_view name, std::function<std::shared_ptr<meta_class>(std::shared_ptr<meta_class>)>&& getter, std::function<void(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>)>&& setter, bool attach = false)
        {
            __add_property(name, type_guid_v<box_value_t<T>>, std::move(getter), std::move(setter), attach);
        }

    public:
        std::unordered_map<std::string, std::unique_ptr<collection_property_info>> const& get_collection_properties() const noexcept { return m_collection_prop_map; }

        XAML_META_API collection_property_info const* get_collection_property(std::string_view name) const noexcept;

    protected:
        XAML_META_API void __add_collection_property(std::string_view name, guid type, std::function<void(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>)>&& adder, std::function<void(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>)>&& remover, bool attach);

    public:
        template <typename T>
        void add_collection_property(std::string_view name, std::function<void(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>)>&& adder, std::function<void(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>)>&& remover, bool attach = false)
        {
            __add_collection_property(name, type_guid_v<box_value_t<T>>, std::move(adder), std::move(remover), attach);
        }

    public:
        XAML_META_API event_info const* get_event(std::string_view name) const noexcept;

        XAML_META_API void add_event(std::string_view name, std::function<std::size_t(std::shared_ptr<meta_class>, type_erased_function const&)>&& adder, std::function<std::size_t(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>, type_erased_this_function const&)>&& adder_erased_this, std::function<void(std::shared_ptr<meta_class>, std::size_t)>&& remover, std::unique_ptr<type_erased_this_function>&& invoker);
    };

    template <typename T, typename TEvent>
    struct __add_event_deduce_helper;

    template <typename T, typename... Args>
    struct __add_event_deduce_helper<T, event<Args...>>
    {
        void operator()(reflection_info& ref, std::string_view name, event<Args...> T::*getter) const
        {
            ref.add_event(
                name,
                std::function<std::size_t(std::shared_ptr<meta_class>, type_erased_function const&)>(
                    [getter](std::shared_ptr<meta_class> self, type_erased_function const& f) -> std::size_t {
                        return (self->shared_from_this<T>().get()->*getter) += [f](Args... args) {
                            f({ box_value(std::forward<Args>(args))... });
                        };
                    }),
                std::function<std::size_t(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>, type_erased_this_function const&)>(
                    [getter](std::shared_ptr<meta_class> self, std::shared_ptr<meta_class> target, type_erased_this_function const& f) -> std::size_t {
                        return (self->shared_from_this<T>().get()->*getter) += [target, f](Args... args) {
                            f(target, { box_value(std::forward<Args>(args))... });
                        };
                    }),
                std::function<void(std::shared_ptr<meta_class>, std::size_t)>(
                    [getter](std::shared_ptr<meta_class> self, std::size_t token) -> void {
                        (self->shared_from_this<T>().get()->*getter) -= token;
                    }),
                make_type_erased_this_function<T, void, Args...>(
                    [getter](std::shared_ptr<T> self, Args... args) {
                        (self.get()->*getter)(std::forward<Args>(args)...);
                    }));
        }
    };

    class enum_reflection_info : public reflection_info_base
    {
    public:
        using reflection_info_base::reflection_info_base;
        ~enum_reflection_info() override {}
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

    struct __hash_path
    {
        std::size_t operator()(std::filesystem::path const& p) const noexcept { return std::filesystem::hash_value(p); }
    };

    class meta_context
    {
    private:
        std::unordered_map<std::filesystem::path, std::unique_ptr<module>, __hash_path> modules_map;

        std::unordered_map<std::string, std::string> namespace_map;
        std::unordered_map<std::string, std::unordered_map<std::string, guid>> type_map;
        std::unordered_map<guid, std::unique_ptr<enum_reflection_info>> enum_map;
        std::unordered_map<guid, std::unique_ptr<reflection_info>> ref_map;
        std::map<std::size_t, std::unique_ptr<__binding_guard>> bind_map;
        std::size_t bind_index{ 0 };

    public:
        XAML_META_API module* add_module(std::filesystem::path const& path);
        std::unordered_map<std::filesystem::path, std::unique_ptr<module>, __hash_path> const& get_modules() const noexcept { return modules_map; }

        XAML_META_API std::string get_real_namespace(std::string_view ns) const;

        // Add a map between xmlns and C++ namespace.
        XAML_META_API void add_xml_namespace(std::string_view xmlns, std::string_view ns) noexcept;

        // Get type with namespace and name.
        XAML_META_API reflection_info const* get_type(std::string_view ns, std::string_view name) const noexcept;

        XAML_META_API reflection_info const* get_type(guid type) const noexcept;

        XAML_META_API void register_type(std::unique_ptr<reflection_info>&& ref) noexcept;

        std::unordered_map<std::string, std::unordered_map<std::string, guid>> const& get_types() const noexcept { return type_map; }

        XAML_META_API enum_reflection_info const* get_enum_type(guid type) const noexcept;

        XAML_META_API void register_type(std::unique_ptr<enum_reflection_info>&& ref) noexcept;

        XAML_META_API bool is_registered_enum(guid type) const noexcept;

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
