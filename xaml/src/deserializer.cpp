#include <xaml/deserializer.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    struct deserializer_markup_context : markup_context
    {
        shared_ptr<meta_class> current;
        shared_ptr<meta_class> current_element() const override { return current; }

        string_view prop;
        string_view current_property() const override { return prop; }

        map<string, shared_ptr<meta_class>>& symbols;
        shared_ptr<meta_class> data_context;
        shared_ptr<meta_class> find_element(string_view name) const override
        {
            if (name.empty())
                return data_context;
            else
            {
                auto it = symbols.find((string)name);
                if (it != symbols.end())
                    return it->second;
                else
                    return nullptr;
            }
        }

        deserializer_markup_context(shared_ptr<meta_class> current, string_view prop, map<string, shared_ptr<meta_class>>& symbols, shared_ptr<meta_class> data_context)
            : current(current), prop(prop), symbols(symbols), data_context(data_context)
        {
        }

        ~deserializer_markup_context() override {}
    };

    XAML_API void deserializer::deserialize_impl(shared_ptr<meta_class> mc, xaml_node& node, shared_ptr<meta_class> root)
    {
        symbols.emplace(node.name, mc);
        for (auto& prop : node.properties)
        {
            auto& value = prop.value;
            switch (value.index())
            {
            case 0: // std::string
                prop.info.set(mc, get<string>(value));
                break;
            case 2: // xaml_node
            {
                auto& node = get<xaml_node>(value);
                auto c = construct(node.type);
                if (c)
                {
                    deserialize_impl(c, node, root);
                    prop.info.set(mc, c);
                }
                else
                {
                    throw xaml_no_default_constructor(node.type);
                }
                break;
            }
            }
        }
        for (auto& prop : node.collection_properties)
        {
            for (auto& n : prop.second.values)
            {
                auto c = construct(n.type);
                if (c)
                {
                    deserialize_impl(c, n, root);
                    prop.second.info.add(mc, c);
                }
                else
                {
                    throw xaml_no_default_constructor(n.type);
                }
            }
        }
        for (auto& ev : node.events)
        {
            auto method = __get_first_method(root->this_type(), ev.value);
            if (method)
            {
                ev.info.add_erased_this(mc, root, method);
            }
            else
            {
                throw xaml_no_member(root->this_type(), ev.value);
            }
        }
    }

    XAML_API void deserializer::deserialize_extensions(xaml_node& node)
    {
        auto mc = symbols[node.name];
        for (auto& prop : node.properties)
        {
            auto& value = prop.value;
            switch (value.index())
            {
            case 1: // markup_node
            {
                auto& n = get<markup_node>(value);
                deserializer_markup_context context{ mc, prop.info.name(), symbols, dynamic_pointer_cast<control>(mc)->get_data_context() };
                auto ex = construct(n.type);
                for (auto& p : n.properties)
                {
                    auto& value = p.value;
                    switch (value.index())
                    {
                    case 0: // std::string
                        p.info.set(ex, get<string>(value));
                        break;
                    }
                }
                dynamic_pointer_cast<markup_extension>(ex)->provide(context);
                break;
            }
            case 2: // xaml_node
                deserialize_extensions(get<xaml_node>(value));
                break;
            }
        }
        for (auto& prop : node.collection_properties)
        {
            for (auto& n : prop.second.values)
            {
                deserialize_extensions(n);
            }
        }
    }

    XAML_API void deserializer::deserialize(shared_ptr<meta_class> mc)
    {
        if (mc)
        {
            xaml_node root_node = reader.parse();
            deserialize_impl(mc, root_node, mc);
            deserialize_extensions(root_node);
        }
    }
} // namespace xaml
