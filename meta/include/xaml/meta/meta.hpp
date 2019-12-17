#ifndef XAML_META_HPP
#define XAML_META_HPP

#include <algorithm>
#include <array>
#include <functional>
#include <initializer_list>
#include <memory>
#include <optional>
#include <string_view>
#include <typeindex>

namespace xaml
{
    struct meta_class
    {
        virtual std::type_index this_type_index() const noexcept = 0;
        virtual ~meta_class() {}
    };

    template <typename TChild>
    struct meta_class_impl : meta_class
    {
        using self_type = TChild;
        std::type_index this_type_index() const noexcept override final { return std::type_index(typeid(TChild)); }
        virtual ~meta_class_impl() override {}
    };

    template <typename... T>
    struct __register_class_helper;

    template <typename T1, typename... Ts>
    struct __register_class_helper<T1, Ts...>
    {
        void operator()() noexcept(noexcept(T1::register_class()) && noexcept(__register_class_helper<Ts...>{}()))
        {
            T1::register_class();
            __register_class_helper<Ts...>{}();
        }
    };

    template <>
    struct __register_class_helper<>
    {
        void operator()() noexcept {}
    };

    template <typename... T>
    void register_class() noexcept(noexcept(__register_class_helper<T...>{}()))
    {
        __register_class_helper<T...>{}();
    }

    std::optional<std::type_index> get_type_index(std::string_view name) noexcept;

    void __register_type(std::string_view name, std::type_index type) noexcept;

    template <typename TChild>
    void register_type(std::string_view name) noexcept
    {
        __register_type(name, std::type_index(typeid(TChild)));
    }

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

        std::function<Return(Args...)> func;
    };

    std::shared_ptr<__type_erased_function> __get_constructor(std::type_index type, std::initializer_list<std::type_index> arg_types) noexcept;

    template <typename... Args>
    std::function<std::shared_ptr<meta_class>(Args...)> get_constructor(std::type_index type) noexcept
    {
        auto ctor = __get_constructor(type, { std::type_index(typeid(Args))... });
        if (ctor)
        {
            return std::function<std::shared_ptr<meta_class>(Args...)>(
                std::reinterpret_pointer_cast<__type_erased_function_impl<std::shared_ptr<meta_class>(Args...)>>(ctor)->func);
        }
        else
        {
            return {};
        }
    }

    template <typename... Args>
    std::shared_ptr<meta_class> construct(std::type_index type, Args... args) noexcept
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

    void __add_constructor(std::type_index type, std::shared_ptr<__type_erased_function> ctor) noexcept;

    template <typename TChild, typename... Args>
    void add_constructor_ex(std::function<std::shared_ptr<meta_class>(Args...)>&& ctor) noexcept
    {
        if (ctor)
        {
            auto c = std::make_shared<__type_erased_function_impl<std::shared_ptr<meta_class>(Args...)>>();
            c->func = ctor;
            __add_constructor(std::type_index(typeid(TChild)), c);
        }
    }
    template <typename TChild, typename... Args>
    void add_constructor() noexcept
    {
        add_constructor_ex<TChild, Args...>(
            std::function<std::shared_ptr<meta_class>(Args...)>(
                [](Args... args) -> std::shared_ptr<meta_class> {
                    return std::make_shared<TChild>(std::forward<Args>(args)...);
                }));
    }

    template <typename T>
    struct __type_erased_this_function_impl;

    template <typename Return, typename... Args>
    struct __type_erased_this_function_impl<Return(Args...)> : __type_erased_function_impl<Return(std::shared_ptr<meta_class>, Args...)>
    {
        ~__type_erased_this_function_impl() override {}

        template <typename T, typename F>
        void set_func(F&& f)
        {
            this->func = std::function<Return(std::shared_ptr<meta_class>, Args...)>(
                [f](std::shared_ptr<meta_class> self, Args... args) -> Return {
                    return std::mem_fn(f)(std::reinterpret_pointer_cast<T>(self).get(), std::forward<Args>(args)...);
                });
        }
    };

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

    std::shared_ptr<__type_erased_function> __get_method(std::type_index type, std::string_view name, std::type_index ret_type, std::initializer_list<std::type_index> arg_types) noexcept;

    template <typename Return, typename... Args>
    std::function<Return(std::shared_ptr<meta_class>, Args...)> get_method(std::type_index type, std::string_view name) noexcept
    {
        auto m = __get_method(type, name, std::type_index(typeid(Return)), { std::type_index(typeid(std::shared_ptr<meta_class>)), std::type_index(typeid(Args))... });
        if (m)
        {
            return std::function<Return(std::shared_ptr<meta_class>, Args...)>(
                std::reinterpret_pointer_cast<__type_erased_this_function_impl<Return(Args...)>>(m)->func);
        }
        else
        {
            return {};
        }
    }

    template <typename Return, typename... Args>
    std::optional<Return> invoke_method(std::shared_ptr<meta_class> obj, std::string_view name, Args... args) noexcept
    {
        auto m = get_method<Return, Args...>(obj->this_type_index(), name);
        if (m)
        {
            return std::make_optional<Return>(m(obj, std::forward<Args>(args)...));
        }
        else
        {
            return std::nullopt;
        }
    }

    void __add_method(std::type_index type, std::string_view name, std::shared_ptr<__type_erased_function> func) noexcept;

    template <typename T, typename TMethod>
    void add_method(std::string_view name, TMethod T::*func) noexcept
    {
        if (func)
        {
            auto f = std::make_shared<__type_erased_this_function_impl<__remove_const_func_t<TMethod>>>();
            f->template set_func<T>(func);
            __add_method(std::type_index(typeid(T)), name, f);
        }
    }

    template <typename T, typename Return, typename... Args>
    void add_method_ex(std::string_view name, std::function<Return(std::shared_ptr<meta_class>, Args...)> func)
    {
        if (func)
        {
            auto f = std::make_shared<__type_erased_this_function_impl<__remove_const_func_t<Return(Args...)>>>();
            f->func = func;
            __add_method(std::type_index(typeid(T)), name, f);
        }
    }
} // namespace xaml

#endif // !XAML_META_HPP
