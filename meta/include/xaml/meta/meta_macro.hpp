#ifndef XAML_META_MACRO_HPP
#define XAML_META_MACRO_HPP

#include <xaml/meta/meta.hpp>

#define REGISTER_TYPE() ::xaml::register_type<self_type>(self_type::class_name)

#define ADD_CTOR(...) ::xaml::add_constructor<self_type, ##__VA_ARGS__>()

#define ADD_METHOD(name) ::xaml::add_method<self_type>(#name, &self_type::name)

#endif // !XAML_META_MACRO_HPP
