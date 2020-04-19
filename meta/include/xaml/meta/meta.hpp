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

} // namespace xaml

#endif // !XAML_META_HPP
