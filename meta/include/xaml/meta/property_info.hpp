#ifndef XAML_META_PROPERTY_INFO_HPP
#define XAML_META_PROPERTY_INFO_HPP

#include <functional>
#include <xaml/box.hpp>
#include <xaml/meta/property_info.h>
#include <xaml/xaml_ptr.hpp>

XAML_META_API xaml_result xaml_property_info_new(xaml_string*, xaml_guid const&, std::function<xaml_result XAML_CALL(xaml_object*, xaml_object**)>&&, std::function<xaml_result XAML_CALL(xaml_object*, xaml_object*)>&&, xaml_property_info**) noexcept;

template <typename T, typename TValueGet, typename TValueSet = TValueGet>
xaml_result xaml_property_info_new(xaml_string* name, xaml_result (T::*XAML_CALL getter)(TValueGet*), xaml_result (T::*XAML_CALL setter)(TValueSet), xaml_property_info**) noexcept
{
    return xaml_property_info_new(
        name, xaml_type_guid_v<T>,
        [getter] XAML_CALL(xaml_object * target, xaml_object * *ptr) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            TValueGet value;
            XAML_RETURN_IF_FAILED(self.get()->*getter(&value));
            return box_value(value, ptr);
        },
        [setter] XAML_CALL(xaml_object * target, xaml_object * obj) -> xaml_result {
            xaml_ptr<T> self;
            XAML_RETURN_IF_FAILED(target->query(&self));
            TValueSet value;
            XAML_RETURN_IF_FAILED(unbox_value(obj, value));
            return self.get()->*setter(value);
        },
        ptr);
}

#endif // !XAML_META_PROPERTY_INFO_HPP
