#ifndef XAML_BOX_HPP
#define XAML_BOX_HPP

#include <xaml/box.h>
#include <xaml/xaml_ptr.hpp>

template <typename T>
inline xaml_result xaml_box_new(T const& value, xaml_box** ptr) noexcept
{
    return xaml_box_new(xaml_type_guid_v<T>, &value, sizeof(T), ptr);
}

template <typename T, typename = void>
struct __box_impl
{
    xaml_result box(T const& value, xaml_object** ptr) const noexcept
    {
        return xaml_box_new(value, (xaml_box**)ptr);
    }

    xaml_result unbox(xaml_object* obj, T& value) const noexcept
    {
        xaml_box* box;
        XAML_RETURN_IF_FAILED(obj->query(&box));
        return box->get_value(value);
    }
};

template <typename T>
struct __box_impl<T*, std::enable_if_t<std::is_base_of_v<xaml_object, T>>>
{
    xaml_result box(T* value, xaml_object** ptr) const noexcept
    {
        return value->query(ptr);
    }

    xaml_result unbox(xaml_object* obj, T*& value) const noexcept
    {
        return obj->query(&value);
    }
};

template <typename T>
struct __box_impl<xaml_ptr<T>, std::enable_if_t<std::is_base_of_v<xaml_object, T>>>
{
    xaml_result box(xaml_ptr<T> const& value, xaml_object** ptr) const noexcept
    {
        return value->query(ptr);
    }

    xaml_result unbox(xaml_object* obj, xaml_ptr<T>& value) const noexcept
    {
        return obj->query(&value);
    }
};

template <typename T>
xaml_result box_value(T const& value, xaml_object** ptr) noexcept
{
    return __box_impl<T>{}.box(value, ptr);
}

template <typename T>
xaml_result unbox_value(xaml_object* obj, T& value) noexcept
{
    return __box_impl<T>{}.unbox(obj, value);
}

#endif // !XAML_BOX_HPP
