#ifndef XAML_UI_META_HPP
#define XAML_UI_META_HPP

#include <any>
#include <xaml/meta/meta.hpp>
#include <xaml/ui/event.hpp>

namespace xaml
{
    struct property_info
    {
    private:
        std::string_view _name;
        std::function<std::any(std::shared_ptr<meta_class>)> getter;
        std::function<void(std::shared_ptr<meta_class>, std::any)> setter;

    public:
        constexpr std::string_view name() const noexcept { return _name; }
        bool can_read() const noexcept { return (bool)getter; }
        bool can_write() const noexcept { return (bool)setter; }

        std::any get(std::shared_ptr<meta_class> const& self) const
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
        void set(std::shared_ptr<meta_class> const& self, std::any value)
        {
            if (setter)
            {
                setter(self, value);
            }
        }

        friend property_info get_property(std::type_index type, std::string_view name);
    };

    inline std::string __property_name(std::string_view name)
    {
        return "prop@" + (std::string)name;
    }

    inline property_info get_property(std::type_index type, std::string_view name)
    {
        property_info info = {};
        info._name = name;
        std::string pname = __property_name(name);
        info.getter = get_method<std::any>(type, pname);
        info.setter = get_method<void, std::any>(type, pname);
        return info;
    }

    template <typename T, typename TValue, typename TGet>
    void add_property_read(std::string_view name, TGet T::*getter)
    {
        if (getter)
        {
            std::string pname = __property_name(name);
            add_method_ex<T, std::any>(
                pname,
                std::function<std::any(std::shared_ptr<meta_class>)>(
                    [getter](std::shared_ptr<meta_class> self) -> std::any {
                        return std::mem_fn(getter)(std::reinterpret_pointer_cast<T>(self).get());
                    }));
        }
    }

    template <typename T, typename TValue, typename TSet>
    void add_property_write(std::string_view name, TSet T::*setter)
    {
        if (setter)
        {
            std::string pname = __property_name(name);
            add_method_ex<T, void, std::any>(
                pname,
                std::function<void(std::shared_ptr<meta_class>, std::any)>(
                    [setter](std::shared_ptr<meta_class> self, std::any value) -> void {
                        std::mem_fn(setter)(std::reinterpret_pointer_cast<T>(self).get(), std::any_cast<TValue>(value));
                    }));
        }
    }

    template <typename T, typename TValue, typename TGet, typename TSet>
    void add_property(std::string_view name, TGet T::*getter, TSet T::*setter)
    {
        add_property_read<T, TValue>(name, getter);
        add_property_write<T, TValue>(name, setter);
    }

    template <typename T, typename TValue>
    void add_property_read_ex(std::string_view name, std::function<TValue(T*)> getter)
    {
        if (getter)
        {
            std::string pname = __property_name(name);
            add_method_ex<T, std::any>(
                pname,
                std::function<std::any(std::shared_ptr<meta_class>)>(
                    [getter](std::shared_ptr<meta_class> self) -> std::any {
                        return getter(std::reinterpret_pointer_cast<T>(self).get());
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
                std::function<void(std::shared_ptr<meta_class>, std::any)>(
                    [setter](std::shared_ptr<meta_class> self, std::any value) -> void {
                        setter(std::reinterpret_pointer_cast<T>(self).get(), std::any_cast<TValue>(value));
                    }));
        }
    }

    template <typename T, typename TValue>
    void add_property_ex(std::string_view name, std::function<TValue(T*)> getter, std::function<void(T*, TValue)> setter)
    {
        add_property_read_ex<T, TValue>(name, getter);
        add_property_write_ex<T, TValue>(name, setter);
    }

    struct event_info
    {
    public:
        using token_type = typename event<>::token_type;

    private:
        std::string_view _name;
        std::function<token_type(std::shared_ptr<meta_class>, std::shared_ptr<__type_erased_function>)> adder;
        std::function<void(std::shared_ptr<meta_class>, token_type)> remover;
        std::shared_ptr<__type_erased_function> invoker;

    public:
        constexpr std::string_view name() const noexcept { return _name; }
        bool can_add() const noexcept { return (bool)adder; }
        bool can_remove() const noexcept { return (bool)remover; }
        bool can_invoke() const noexcept { return (bool)invoker; }

        template <typename... Args>
        bool match_arg_type()
        {
            return invoker && invoker->is_same_arg_type<std::shared_ptr<meta_class>, Args...>();
        }

        template <typename... Args>
        token_type add(std::shared_ptr<meta_class> const& self, std::function<void(Args...)> handler)
        {
            if (adder && match_arg_type<Args...>())
            {
                auto h = std::make_shared<__type_erased_function_impl<void(Args...)>>();
                h->func = handler;
                return adder(self, h);
            }
            return 0;
        }

        void remove(std::shared_ptr<meta_class> const& self, token_type token)
        {
            if (remover)
            {
                remover(self, token);
            }
        }

        template <typename... Args>
        void invoke(std::shared_ptr<meta_class> const& self, Args... args)
        {
            if (match_arg_type<Args...>())
            {
                auto i = std::reinterpret_pointer_cast<__type_erased_function_impl<void(std::shared_ptr<meta_class>, Args...)>>(invoker);
                i->func(self, std::forward<Args>(args)...);
            }
        }

        friend event_info __get_event(std::type_index type, std::string_view name, std::initializer_list<std::type_index> arg_types);
    };

    inline std::string __event_name(std::string_view name)
    {
        return "event@" + (std::string)name;
    }

    inline event_info __get_event(std::type_index type, std::string_view name, std::initializer_list<std::type_index> arg_types)
    {
        event_info info = {};
        info._name = name;
        std::string ename = __event_name(name);
        info.adder = get_method<typename event_info::token_type, std::shared_ptr<__type_erased_function>>(type, ename);
        info.remover = get_method<void, typename event_info::token_type>(type, ename);
        info.invoker = __get_method(type, ename, std::type_index(typeid(void)), arg_types);
        return info;
    }

    template <typename... Args>
    event_info get_event(std::type_index type, std::string_view name)
    {
        return __get_event(type, name, { std::type_index(typeid(std::shared_ptr<meta_class>)), std::type_index(typeid(Args))... });
    }

    template <typename T, typename... Args, typename TAdd>
    void add_event_add(std::string_view name, TAdd T::*adder)
    {
        if (adder)
        {
            std::string ename = __event_name(name);
            add_method_ex<T, typename event_info::token_type, std::shared_ptr<__type_erased_function>>(
                ename,
                std::function<typename event_info::token_type(std::shared_ptr<meta_class>, std::shared_ptr<__type_erased_function>)>(
                    [adder](std::shared_ptr<meta_class> self, std::shared_ptr<__type_erased_function> handler) -> typename event_info::token_type {
                        if (handler->is_same_arg_type<Args...>())
                        {
                            auto h = std::reinterpret_pointer_cast<__type_erased_function_impl<void(Args...)>>(handler);
                            return std::mem_fn(adder)(std::reinterpret_pointer_cast<T>(self).get(), std::move(h->func));
                        }
                        return 0;
                    }));
        }
    }

    template <typename T, typename... Args, typename TRemove>
    void add_event_remove(std::string_view name, TRemove T::*remover)
    {
        if (remover)
        {
            std::string ename = __event_name(name);
            add_method_ex<T, void, typename event_info::token_type>(
                ename,
                std::function<void(std::shared_ptr<meta_class>, typename event_info::token_type)>(
                    [remover](std::shared_ptr<meta_class> self, typename event_info::token_type token) -> void {
                        std::mem_fn(remover)(std::reinterpret_pointer_cast<T>(self).get(), token);
                    }));
        }
    }

    template <typename T, typename... Args, typename TInvoke>
    void add_event_invoke(std::string_view name, TInvoke T::*invoker)
    {
        if (invoker)
        {
            std::string ename = __event_name(name);
            add_method_ex<T, void, Args...>(
                ename,
                std::function<void(std::shared_ptr<meta_class>, Args...)>(
                    [invoker](std::shared_ptr<meta_class> self, Args... args) -> void {
                        std::mem_fn(invoker)(std::reinterpret_pointer_cast<T>(self).get(), std::forward<Args>(args)...);
                    }));
        }
    }

    template <typename T, typename... Args, typename TAdd, typename TRemove, typename TInvoke>
    void add_event(std::string_view name, TAdd T::*adder, TRemove T::*remover, TInvoke T::*invoker)
    {
        add_event_add<T, Args...>(name, adder);
        add_event_remove<T, Args...>(name, remover);
        add_event_invoke<T, Args...>(name, invoker);
    }
} // namespace xaml

#endif // !XAML_UI_META_HPP
