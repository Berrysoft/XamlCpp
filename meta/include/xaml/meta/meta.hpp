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
    };

    template <typename TChild>
    struct meta_class_impl : meta_class
    {
        std::type_index this_type_index() const noexcept override final { return std::type_index(typeid(TChild)); }
    };

    std::optional<std::type_index> get_type_index(std::string_view name);

    void __register_type(std::string_view name, std::type_index type);

    template <typename TChild>
    void register_type(std::string_view name)
    {
        __register_type(name, std::type_index(typeid(TChild)));
    }

    namespace details
    {
        struct type_erased_function
        {
            virtual bool is_same_arg_type(std::initializer_list<std::type_index> types) const noexcept = 0;
            virtual bool is_same_return_type(std::type_index type) const noexcept = 0;
            virtual ~type_erased_function() {}
        };

        template <typename T>
        struct type_erased_function_impl;

        template <typename Return, typename... Args>
        struct type_erased_function_impl<Return(Args...)> : type_erased_function
        {
        private:
            static inline bool is_same_arg_type_impl(std::initializer_list<std::type_index> lhs, std::initializer_list<std::type_index> rhs)
            {
                return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
            }

        public:
            bool is_same_arg_type(std::initializer_list<std::type_index> types) const noexcept override
            {
                return is_same_arg_type_impl({ std::type_index(typeid(Args))... }, types);
            }
            bool is_same_return_type(std::type_index type) const noexcept override
            {
                return type == std::type_index(typeid(Return));
            }
            ~type_erased_function_impl() override {}

            std::function<Return(Args...)> func;
        };
    } // namespace details

    std::shared_ptr<details::type_erased_function> __get_constructor(std::type_index type, std::initializer_list<std::type_index> arg_types);

    template <typename... Args>
    std::optional<std::function<std::shared_ptr<meta_class>(Args...)>> get_constructor(std::type_index type)
    {
        auto ctor = __get_constructor(type, { std::type_index(typeid(Args))... });
        if (ctor)
        {
            return std::make_optional<std::function<std::shared_ptr<meta_class>(Args...)>>(std::reinterpret_pointer_cast<details::type_erased_function_impl<std::shared_ptr<meta_class>(Args...)>>(ctor)->func);
        }
        else
        {
            return std::nullopt;
        }
    }

    template <typename... Args>
    std::shared_ptr<meta_class> construct(std::type_index type, Args... args)
    {
        auto ctor = get_constructor<Args...>(type);
        if (ctor)
        {
            return (*ctor)(std::forward<Args>(args)...);
        }
        else
        {
            return nullptr;
        }
    }

    void __add_constructor(std::type_index type, std::shared_ptr<details::type_erased_function> ctor);

    template <typename TChild, typename... Args>
    void add_constructor_ex(std::function<std::shared_ptr<meta_class>(Args...)>&& ctor)
    {
        auto c = std::make_shared<details::type_erased_function_impl<std::shared_ptr<meta_class>(Args...)>>();
        c->func = ctor;
        __add_constructor(std::type_index(typeid(TChild)), c);
    }
    template <typename TChild, typename... Args>
    void add_constructor()
    {
        add_constructor_ex<TChild, Args...>(std::function<std::shared_ptr<meta_class>(Args...)>([](Args... args) -> std::shared_ptr<meta_class> { return std::make_shared<TChild>(std::forward<Args>(args)...); }));
    }

    std::shared_ptr<details::type_erased_function> __get_method(std::type_index type, std::string_view name, std::type_index ret_type, std::initializer_list<std::type_index> arg_types);

    template <typename Return, typename... Args>
    std::optional<std::function<Return(std::shared_ptr<meta_class>, Args...)>> get_method(std::type_index type, std::string_view name)
    {
        auto m = __get_method(type, name, std::type_index(typeid(Return)), { std::type_index(typeid(std::shared_ptr<meta_class>)), std::type_index(typeid(Args))... });
        if (m)
        {
            return std::make_optional<std::function<Return(std::shared_ptr<meta_class>, Args...)>>(std::reinterpret_pointer_cast<details::type_erased_function_impl<Return(std::shared_ptr<meta_class>, Args...)>>(m)->func);
        }
        else
        {
            return std::nullopt;
        }
    }

    template <typename Return, typename... Args>
    std::optional<Return> invoke_method(std::shared_ptr<meta_class> obj, std::string_view name, Args... args)
    {
        auto m = get_method<Return, Args...>(obj->this_type_index(), name);
        if (m)
        {
            return std::make_optional<Return>((*m)(obj, std::forward<Args>(args)...));
        }
        else
        {
            return std::nullopt;
        }
    }

    void __add_method(std::type_index type, std::string_view name, std::shared_ptr<details::type_erased_function> func);

    template <typename T, typename Return, typename... Args>
    void add_method(std::string_view name, Return (T::*func)(Args...))
    {
        auto f = std::make_shared<details::type_erased_function_impl<Return(std::shared_ptr<meta_class>, Args...)>>();
        f->func = std::function<Return(std::shared_ptr<meta_class>, Args...)>([func](std::shared_ptr<meta_class> self, Args... args) -> Return { return std::mem_fn(func)((T*)self.get(), std::forward<Args>(args)...); });
        __add_method(std::type_index(typeid(T)), name, f);
    }
} // namespace xaml

#endif // !XAML_META_HPP
