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
#include <typeindex>
#include <unordered_map>
#include <xaml/array_view.hpp>
#include <xaml/meta/event.hpp>
#include <xaml/meta/guid.hpp>
#include <xaml/meta/module.hpp>
#include <xaml/utility.hpp>

namespace xaml
{
    // BASE TYPE

    struct meta_class;

    template <>
    struct type_guid<meta_class>
    {
        static constexpr guid value{ 0xaf86e2e0, 0xb12d, 0x4c6a, { 0x9c, 0x5a, 0xd7, 0xaa, 0x65, 0x10, 0x1e, 0x90 } };
    };

    // Base class of all classes which is registered for reflection.
    struct meta_class : std::enable_shared_from_this<meta_class>
    {
        virtual ~meta_class() {}
        virtual guid get_type() const noexcept = 0;
        virtual bool query_type(guid const& t) const noexcept { return t == type_guid_v<meta_class>; }

        template <typename T>
        std::shared_ptr<T> shared_from_this()
        {
            return std::static_pointer_cast<T>(std::enable_shared_from_this<meta_class>::shared_from_this());
        }

        template <typename T>
        std::shared_ptr<T const> shared_from_this() const
        {
            return std::static_pointer_cast<T const>(std::enable_shared_from_this<meta_class>::shared_from_this());
        }

        template <typename T>
        std::shared_ptr<T> query() noexcept
        {
            if (query_type(type_guid_v<T>))
                return shared_from_this<T>();
            else
                return nullptr;
        }

        template <typename T>
        std::shared_ptr<T const> query() const noexcept
        {
            if (query_type(type_guid_v<T>))
                return shared_from_this<T>();
            else
                return nullptr;
        }
    };

#define META_CLASS_IMPL(base)                                                                                        \
    ::xaml::guid get_type() const noexcept override { return ::xaml::type_guid_v<::std::decay_t<decltype(*this)>>; } \
    bool query_type(::xaml::guid const& t) const noexcept override { return t == ::xaml::type_guid_v<::std::decay_t<decltype(*this)>> || base::query_type(t); }

    template <typename T>
    struct meta_box;

    template <>
    struct type_guid<meta_box<bool>>
    {
        static constexpr guid value{ 0xc3a0fdbf, 0xa30b, 0x315e, { 0xb0, 0x19, 0x42, 0xab, 0xac, 0xf7, 0x2c, 0xae } };
    };

    template <>
    struct type_guid<meta_box<char>>
    {
        static constexpr guid value{ 0x829b06f1, 0x9a66, 0x44f6, { 0xb9, 0x13, 0x4a, 0x7e, 0x75, 0x30, 0xc3, 0x8a } };
    };

    template <>
    struct type_guid<meta_box<wchar_t>>
    {
        static constexpr guid value{ 0x2d08eb84, 0x64e6, 0x3688, { 0x80, 0xd7, 0xe0, 0xc5, 0x48, 0xac, 0x36, 0x2d } };
    };

    template <>
    struct type_guid<meta_box<signed char>>
    {
        static constexpr guid value{ 0xdc3c96bc, 0x48ce, 0x3ef7, { 0x8c, 0x15, 0x37, 0xbc, 0x7e, 0xc8, 0x07, 0xa1 } };
    };

    template <>
    struct type_guid<meta_box<short>>
    {
        static constexpr guid value{ 0xe3e8f6e4, 0xe677, 0x3436, { 0x8e, 0x0e, 0x96, 0x3d, 0x3d, 0x6f, 0x81, 0x0a } };
    };

    template <>
    struct type_guid<meta_box<int>>
    {
        static constexpr guid value{ 0x0ce68e8c, 0x51a4, 0x3115, { 0xbe, 0x13, 0xdd, 0x1a, 0xce, 0x42, 0xe6, 0x1f } };
    };

    template <>
    struct type_guid<meta_box<long>>
    {
        static constexpr guid value{ 0x78c952b3, 0x23a5, 0x429f, { 0x84, 0xaf, 0x5c, 0xa9, 0x2b, 0x49, 0xf9, 0xfb } };
    };

    template <>
    struct type_guid<meta_box<long long>>
    {
        static constexpr guid value{ 0xf940ae92, 0xb495, 0x342e, { 0xab, 0x07, 0xe5, 0xff, 0x86, 0x3f, 0xb4, 0x15 } };
    };

    template <>
    struct type_guid<meta_box<unsigned char>>
    {
        static constexpr guid value{ 0xb3b3b9f4, 0x12da, 0x3efd, { 0xb1, 0x31, 0xdf, 0x0e, 0x1f, 0x9f, 0x91, 0x4b } };
    };

    template <>
    struct type_guid<meta_box<unsigned short>>
    {
        static constexpr guid value{ 0x33d9552a, 0xdc77, 0x3314, { 0x8e, 0xa3, 0x80, 0xbf, 0x05, 0x8e, 0xa1, 0x63 } };
    };

    template <>
    struct type_guid<meta_box<unsigned int>>
    {
        static constexpr guid value{ 0xe38cb0a1, 0x90ae, 0x3c05, { 0x9b, 0x2e, 0x06, 0x2a, 0x58, 0x4d, 0x6c, 0x50 } };
    };

    template <>
    struct type_guid<meta_box<unsigned long>>
    {
        static constexpr guid value{ 0x089d52f5, 0xc09d, 0x4be7, { 0xa1, 0xce, 0x58, 0xf4, 0x54, 0x8b, 0xe8, 0x59 } };
    };

    template <>
    struct type_guid<meta_box<unsigned long long>>
    {
        static constexpr guid value{ 0x5f5fc118, 0x3099, 0x32f3, { 0xab, 0xba, 0x75, 0x57, 0x06, 0xf8, 0xb7, 0xf8 } };
    };

    template <>
    struct type_guid<meta_box<float>>
    {
        static constexpr guid value{ 0xa0782160, 0xfcb5, 0x30dc, { 0xb7, 0x5c, 0xb7, 0xc3, 0x04, 0xdf, 0xc3, 0xd3 } };
    };

    template <>
    struct type_guid<meta_box<double>>
    {
        static constexpr guid value{ 0x9144b7d6, 0x3d5f, 0x3b29, { 0x81, 0x31, 0xff, 0x0d, 0xb5, 0x51, 0xe1, 0x7c } };
    };

    template <>
    struct type_guid<meta_box<long double>>
    {
        static constexpr guid value{ 0x3b0b7aa1, 0xe4c0, 0x3eab, { 0x8e, 0x85, 0xbc, 0xca, 0x45, 0xb7, 0x3a, 0xaf } };
    };

    template <>
    struct type_guid<meta_box<std::string>>
    {
        static constexpr guid value{ 0x2cadd284, 0xb5f9, 0x4dfd, { 0xb8, 0x55, 0xf6, 0x47, 0x41, 0x23, 0xf8, 0x0f } };
    };

    template <>
    struct type_guid<meta_box<std::wstring>>
    {
        static constexpr guid value{ 0xc8386ec4, 0xd28d, 0x422f, { 0x9e, 0x44, 0x36, 0xaa, 0x77, 0x63, 0x39, 0xd3 } };
    };

    template <typename T>
    struct meta_box : public meta_class
    {
    public:
        META_CLASS_IMPL(meta_class)

    private:
        T m_value{};

    public:
        meta_box() {}
        meta_box(T const& value) : m_value(value) {}
        meta_box(T&& value) : m_value(std::move(value)) {}
        ~meta_box() override {}

        meta_box& operator=(T const& value)
        {
            m_value = value;
            return *this;
        }

        meta_box& operator=(T&& value)
        {
            m_value = std::move(value);
            return *this;
        }

        operator T() const noexcept { return m_value; }

        T& get() noexcept { return m_value; }
        T const& get() const noexcept { return m_value; }
    };

    template <typename Char, typename Traits, typename Allocator>
    struct meta_box<std::basic_string<Char, Traits, Allocator>> : public meta_class
    {
    public:
        META_CLASS_IMPL(meta_class)

    private:
        std::basic_string<Char, Traits, Allocator> m_value{};

    public:
        meta_box() {}
        meta_box(std::basic_string_view<Char, Traits> value) : m_value(value) {}
        ~meta_box() override {}

        meta_box& operator=(std::basic_string_view<Char, Traits> value)
        {
            m_value = value;
            return *this;
        }

        operator std::basic_string<Char, Traits, Allocator>() const noexcept { return m_value; }

        std::basic_string_view<Char, Traits> get() const noexcept { return m_value; }
    };

    template <typename T, typename Allocator>
    struct meta_box<std::vector<T, Allocator>> : public meta_class
    {
    public:
        META_CLASS_IMPL(meta_class)

    private:
        std::vector<T, Allocator> m_value{};

    public:
        meta_box() {}
        meta_box(array_view<T> value) : m_value(value) {}
        ~meta_box() override {}

        meta_box& operator=(array_view<T> value)
        {
            m_value = value;
            return *this;
        }

        operator std::vector<T, Allocator>() const noexcept { return m_value; }

        array_view<T> get() const noexcept { return m_value; }
    };

    template <typename T, typename = void>
    struct __box_helper
    {
        using type = meta_box<T>;

        std::shared_ptr<type> operator()(T value)
        {
            return std::make_shared<meta_box<T>>(std::move(value));
        }
    };

    template <>
    struct __box_helper<void, void>
    {
        using type = void;

        type operator()(void) {}
    };

    template <typename T>
    struct __box_helper<std::shared_ptr<T>, std::enable_if_t<std::is_base_of_v<meta_class, T>>>
    {
        using type = T;

        std::shared_ptr<type> operator()(std::shared_ptr<T> value)
        {
            return value;
        }
    };

    template <typename T>
    struct __is_valid_char : std::false_type
    {
    };

    template <>
    struct __is_valid_char<char> : std::true_type
    {
    };

    template <>
    struct __is_valid_char<wchar_t> : std::true_type
    {
    };

    template <typename T>
    constexpr bool __is_valid_char_v = __is_valid_char<T>::value;

    template <typename Char>
    struct __box_helper<Char const*, std::enable_if_t<__is_valid_char_v<Char>>>
    {
        using type = meta_box<std::basic_string<Char>>;

        std::shared_ptr<type> operator()(Char const* str)
        {
            return std::make_shared<meta_box<std::basic_string<Char>>>(str);
        }
    };

    template <typename Char, typename Traits>
    struct __box_helper<std::basic_string_view<Char, Traits>, void>
    {
        using type = meta_box<std::basic_string<Char, Traits>>;

        std::shared_ptr<type> operator()(std::basic_string_view<Char, Traits> str)
        {
            return std::make_shared<meta_box<std::basic_string<Char>>>(str);
        }
    };

    template <typename T>
    struct __box_helper<array_view<T>, void>
    {
        using type = meta_box<std::vector<T>>;

        std::shared_ptr<type> operator()(array_view<T> view)
        {
            return std::make_shared<meta_box<std::vector<T>>>(view);
        }
    };

    template <typename T>
    decltype(auto) box_value(T&& value)
    {
        return __box_helper<std::decay_t<T>>{}(std::forward<T>(value));
    }

    template <typename T>
    decltype(auto) unbox_value(std::shared_ptr<meta_class> value);

    template <typename T>
    using box_value_t = typename __box_helper<T>::type;

    struct type_erased_function
    {
        guid return_type{};
        std::vector<guid> args_type;
        std::function<std::shared_ptr<meta_class>(array_view<std::shared_ptr<meta_class>>)> func;

        bool is_same_return_type(guid t) const noexcept { return return_type == t; }
        bool is_same_arg_type(std::initializer_list<guid> ts) const noexcept
        {
            return std::equal(args_type.begin(), args_type.end(), ts.begin(), ts.end());
        }

        template <typename Return>
        bool is_same_return_type() const noexcept
        {
            return is_same_return_type(type_guid_v<Return>);
        }

        template <typename... Args>
        bool is_same_arg_type() const noexcept
        {
            return is_same_arg_type({ type_guid_v<Args>... });
        }

        operator bool() const { return (bool)func; }

        std::shared_ptr<meta_class> operator()(array_view<std::shared_ptr<meta_class>> args) const { return func(args); }
        std::shared_ptr<meta_class> operator()(std::initializer_list<std::shared_ptr<meta_class>> args) const
        {
            std::vector<std::shared_ptr<meta_class>> args_vec{ args };
            return func(args_vec);
        }
    };

    struct type_erased_this_function
    {
        guid return_type{};
        std::vector<guid> args_type;
        std::function<std::shared_ptr<meta_class>(std::shared_ptr<meta_class>, array_view<std::shared_ptr<meta_class>>)> func;

        bool is_same_return_type(guid const& t) const noexcept { return return_type == t; }
        bool is_same_arg_type(std::initializer_list<guid> ts) const noexcept
        {
            return std::equal(args_type.begin(), args_type.end(), ts.begin(), ts.end());
        }

        template <typename Return>
        bool is_same_return_type() const noexcept
        {
            return is_same_return_type(type_guid_v<Return>);
        }

        template <typename... Args>
        bool is_same_arg_type() const noexcept
        {
            return is_same_arg_type({ type_guid_v<Args>... });
        }

        operator bool() const { return (bool)func; }

        std::shared_ptr<meta_class> operator()(std::shared_ptr<meta_class> self, array_view<std::shared_ptr<meta_class>> args) const { return func(self, args); }
        std::shared_ptr<meta_class> operator()(std::shared_ptr<meta_class> self, std::initializer_list<std::shared_ptr<meta_class>> args) const
        {
            std::vector<std::shared_ptr<meta_class>> args_vec{ args };
            return func(self, args_vec);
        }
    };

    template <typename Return, typename... Args, typename F, std::size_t... Indicies>
    std::unique_ptr<type_erased_function> __make_type_erased_function_impl(F&& f, std::index_sequence<Indicies...>)
    {
        auto func = std::make_unique<type_erased_function>();
        func->return_type = type_guid_v<box_value_t<Return>>;
        func->args_type = { type_guid_v<box_value_t<Args>>... };
        if constexpr (std::is_same_v<Return, void>)
            func->func = [f](array_view<std::shared_ptr<meta_class>> args) -> std::shared_ptr<meta_class> { f(unbox_value<Args>(args[Indicies])...); return {}; };
        else
            func->func = [f](array_view<std::shared_ptr<meta_class>> args) -> std::shared_ptr<meta_class> { return box_value(f(unbox_value<Args>(args[Indicies])...)); };
        return func;
    }

    template <typename Return, typename... Args>
    std::unique_ptr<type_erased_function> make_type_erased_function(Return (*f)(Args...))
    {
        return __make_type_erased_function_impl<Return, Args...>(f, std::make_index_sequence<sizeof...(Args)>{});
    }

    template <typename Return, typename... Args, typename F>
    std::unique_ptr<type_erased_function> make_type_erased_function(F&& f)
    {
        return __make_type_erased_function_impl<Return, Args...>(std::forward<F>(f), std::make_index_sequence<sizeof...(Args)>{});
    }

    template <typename Class, typename Return, typename... Args, typename F, std::size_t... Indicies>
    std::unique_ptr<type_erased_this_function> __make_type_erased_this_function_impl(F&& f, std::index_sequence<Indicies...>)
    {
        auto func = std::make_unique<type_erased_this_function>();
        func->return_type = type_guid_v<box_value_t<Return>>;
        func->args_type = { type_guid_v<box_value_t<Args>>... };
        if constexpr (std::is_same_v<Return, void>)
            func->func = [f](std::shared_ptr<meta_class> self, array_view<std::shared_ptr<meta_class>> args) -> std::shared_ptr<meta_class> { f(std::static_pointer_cast<Class>(self), unbox_value<Args>(args[Indicies])...); return {}; };
        else
            func->func = [f](std::shared_ptr<meta_class> self, array_view<std::shared_ptr<meta_class>> args) -> std::shared_ptr<meta_class> { return box_value(f(std::static_pointer_cast<Class>(self), unbox_value<Args>(args[Indicies])...)); };
        return func;
    }

    template <typename Class, typename Return, typename... Args>
    std::unique_ptr<type_erased_this_function> make_type_erased_this_function(Return (Class::*f)(Args...))
    {
        return __make_type_erased_this_function_impl<Class, Return, Args...>(std::mem_fn(f), std::make_index_sequence<sizeof...(Args)>{});
    }

    template <typename Class, typename Return, typename... Args, typename F>
    std::unique_ptr<type_erased_this_function> make_type_erased_this_function(F&& f)
    {
        return __make_type_erased_this_function_impl<Class, Return, Args...>(std::forward<F>(f), std::make_index_sequence<sizeof...(Args)>{});
    }

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
        std::function<token_type(std::shared_ptr<meta_class>, type_erased_function const&)> adder;
        std::function<token_type(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>, type_erased_this_function const&)> adder_this;
        std::function<void(std::shared_ptr<meta_class>, token_type)> remover;
        std::unique_ptr<type_erased_this_function> invoker;

    public:
        std::string_view name() const noexcept { return m_name; }
        bool can_add() const noexcept { return (bool)adder; }
        bool can_remove() const noexcept { return (bool)remover; }
        bool can_invoke() const noexcept { return (bool)invoker; }

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
            invoker(self, { std::forward<Args>(args)... });
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

    protected:
        XAML_META_API void __add_event(std::string_view name, std::function<std::size_t(std::shared_ptr<meta_class>, type_erased_function const&)>&& adder, std::function<std::size_t(std::shared_ptr<meta_class>, std::shared_ptr<meta_class>, type_erased_this_function const&)>&& adder_erased_this, std::function<void(std::shared_ptr<meta_class>, std::size_t)>&& remover, std::unique_ptr<type_erased_this_function>&& invoker);

    public:
        template <typename... Args>
        void add_event(std::string_view name, std::function<std::size_t(std::shared_ptr<meta_class>, type_erased_function const&)>&& adder, std::function<void(std::shared_ptr<meta_class>, std::size_t)>&& remover, std::unique_ptr<type_erased_this_function>&& invoker)
        {
            if (invoker)
            {
                __add_event(
                    name, std::move(adder),
                    [adder](std::shared_ptr<meta_class> self, std::shared_ptr<meta_class> target, type_erased_this_function const& func) -> std::size_t {
                        return adder(self, *make_type_erased_function<void, Args...>([target, &func](Args... args) { func(target, { box_value(std::forward<Args>(args))... }); }));
                    },
                    std::move(remover), std::move(invoker));
            }
        }
    };

    template <typename T, typename TEvent>
    struct __add_event_deduce_helper;

    template <typename T, typename... Args>
    struct __add_event_deduce_helper<T, event<Args...>>
    {
        void operator()(reflection_info& ref, std::string_view name, std::function<std::size_t(std::shared_ptr<meta_class>, std::function<void(Args...)>)>&& adder, std::function<void(std::shared_ptr<meta_class>, std::size_t)>&& remover, event<Args...> T::*getter) const
        {
            ref.add_event<Args...>(
                name,
                std::function<std::size_t(std::shared_ptr<meta_class>, type_erased_function const&)>([adder](std::shared_ptr<meta_class> self, type_erased_function const& f) -> std::size_t { return adder(self, [f](Args... args) { f({ box_value(std::forward<Args>(args))... }); }); }),
                std::move(remover),
                make_type_erased_this_function<T, void, Args...>([getter](std::shared_ptr<T> self, Args... args) { (self.get()->*getter)(std::forward<Args>(args)...); }));
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
