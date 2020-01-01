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

    void deserializer::deserialize_impl(shared_ptr<meta_class> mc, xaml_node& node, shared_ptr<meta_class> root)
    {
        symbols.emplace(node.name, mc);
        for (auto& prop : node.properties)
        {
            prop.info.set(mc, prop.value);
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
        for (auto& prop : node.construct_properties)
        {
            auto c = construct(prop.value.type);
            if (c)
            {
                deserialize_impl(c, prop.value, root);
                prop.info.set(mc, c);
            }
            else
            {
                throw xaml_no_default_constructor(prop.value.type);
            }
        }
        bool is_container = invoke_method<bool>(mc, "is_container").value_or(false);
        bool is_multicontainer = invoke_method<bool>(mc, "is_multicontainer").value_or(false);
        if (is_container)
        {
            if (node.children.size() > 1 && !is_multicontainer)
            {
                throw xaml_not_multicontainer(mc->this_type());
            }
            for (auto& cnode : node.children)
            {
                auto c = construct(cnode.type);
                if (c)
                {
                    deserialize_impl(c, cnode, root);
                    if (is_multicontainer)
                    {
                        invoke_method<void>(mc, "add_child", dynamic_pointer_cast<control>(c));
                    }
                    else
                    {
                        auto prop = get_property(node.type, "child");
                        prop.set(mc, dynamic_pointer_cast<control>(c));
                    }
                }
                else
                {
                    throw xaml_no_default_constructor(cnode.type);
                }
            }
        }
    }

    void deserializer::deserialize_extensions(xaml_node& node)
    {
        auto mc = symbols[node.name];
        for (auto& prop : node.extension_properties)
        {
            deserializer_markup_context context{ mc, prop.info.name(), symbols, dynamic_pointer_cast<control>(mc)->get_data_context() };
            prop.value->provide(context);
        }
        for (auto& c : node.children)
        {
            deserialize_extensions(c);
        }
    }

    void deserializer::deserialize(shared_ptr<meta_class> mc)
    {
        if (mc)
        {
            xaml_node root_node = reader.parse();
            deserialize_impl(mc, root_node, mc);
            deserialize_extensions(root_node);
        }
    }
} // namespace xaml
