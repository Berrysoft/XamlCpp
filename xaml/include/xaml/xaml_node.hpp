#ifndef XAML_XAML_NODE_HPP
#define XAML_XAML_NODE_HPP

#include <map>
#include <optional>
#include <string>
#include <tuple>
#include <typeindex>
#include <variant>
#include <vector>
#include <xaml/meta/meta.hpp>

namespace xaml
{
    struct xaml_property;
    struct xaml_collection_property;

    struct markup_node
    {
        std::type_index type;
        std::string name;
        std::vector<xaml_property> properties;
    };

    struct xaml_event
    {
        event_info info;
        std::string value;
    };

    struct xaml_node
    {
        std::type_index type;
        std::string name;
        std::optional<std::tuple<std::string, std::string>> map_class;
        std::vector<xaml_property> properties;
        std::map<std::string, xaml_collection_property> collection_properties;
        std::vector<xaml_event> events;
    };

    struct xaml_property
    {
        std::type_index host_type{ typeid(std::nullptr_t) };
        property_info info;
        std::variant<std::string, markup_node, xaml_node> value;
    };

    struct xaml_collection_property
    {
        std::type_index host_type{ typeid(std::nullptr_t) };
        collection_property_info info;
        std::vector<xaml_node> values;
    };
} // namespace xaml

#endif // !XAML_XAML_NODE_HPP
