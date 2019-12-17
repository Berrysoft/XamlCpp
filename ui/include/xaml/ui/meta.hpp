#ifndef XAML_UI_META_HPP
#define XAML_UI_META_HPP

#include <any>
#include <xaml/meta/meta.hpp>

namespace xaml
{
    struct property_info
    {
    private:
        std::string_view _name;
        std::optional<std::function<std::any(std::shared_ptr<meta_class>)>> getter;
        std::optional<std::function<void(std::shared_ptr<meta_class>, std::any)>> setter;

    public:
        constexpr std::string_view name() const noexcept { return _name; }
        constexpr bool can_read() const noexcept { return (bool)getter; }
        constexpr bool can_write() const noexcept { return (bool)setter; }
        std::any get(std::shared_ptr<meta_class> const& self) const
        {
            if (getter)
            {
                return (*getter)(self);
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
                (*setter)(self, value);
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
            add_method_ex<T, std::any>(pname, std::function<std::any(std::shared_ptr<meta_class>)>([getter](std::shared_ptr<meta_class> self) -> std::any { return std::mem_fn(getter)(std::reinterpret_pointer_cast<T>(self).get()); }));
        }
    }

    template <typename T, typename TValue, typename TSet>
    void add_property_write(std::string_view name, TSet T::*setter)
    {
        if (setter)
        {
            std::string pname = __property_name(name);
            add_method_ex<T, void, std::any>(pname, std::function<void(std::shared_ptr<meta_class>, std::any)>([setter](std::shared_ptr<meta_class> self, std::any value) -> void { std::mem_fn(setter)(std::reinterpret_pointer_cast<T>(self).get(), std::any_cast<TValue>(value)); }));
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
            add_method_ex<T, std::any>(pname, std::function<std::any(std::shared_ptr<meta_class>)>([getter](std::shared_ptr<meta_class> self) -> std::any { return getter(std::reinterpret_pointer_cast<T>(self).get()); }));
        }
    }

    template <typename T, typename TValue>
    void add_property_write_ex(std::string_view name, std::function<void(T*, TValue)> setter)
    {
        if (setter)
        {
            std::string pname = __property_name(name);
            add_method_ex<T, void, std::any>(pname, std::function<void(std::shared_ptr<meta_class>, std::any)>([setter](std::shared_ptr<meta_class> self, std::any value) -> void { setter(std::reinterpret_pointer_cast<T>(self).get(), std::any_cast<TValue>(value)); }));
        }
    }

    template <typename T, typename TValue>
    void add_property_ex(std::string_view name, std::function<TValue(T*)> getter, std::function<void(T*, TValue)> setter)
    {
        add_property_read_ex<T, TValue>(name, getter);
        add_property_write_ex<T, TValue>(name, setter);
    }
} // namespace xaml

#endif // !XAML_UI_META_HPP
