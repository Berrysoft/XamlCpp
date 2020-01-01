#include <xaml/deserializer.hpp>
#include <xaml/ui/control.hpp>

using namespace std;

namespace xaml
{
    void deserializer::deserialize_impl(shared_ptr<meta_class> mc, xaml_node& node, shared_ptr<meta_class> root)
    {
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
            for (auto& node : node.children)
            {
                auto c = construct(node.type);
                if (c)
                {
                    deserialize_impl(c, node, root);
                    if (is_multicontainer)
                    {
                        invoke_method<void>(mc, "add_child", dynamic_pointer_cast<control>(c));
                    }
                    else
                    {
                        auto prop = get_property(mc->this_type(), "child");
                        prop.set(mc, dynamic_pointer_cast<control>(c));
                    }
                }
                else
                {
                    throw xaml_no_default_constructor(node.type);
                }
            }
        }
    }

    void deserializer::deserialize(shared_ptr<meta_class> mc)
    {
        if (mc)
        {
            xaml_node root_node = reader.parse();
            deserialize_impl(mc, root_node, mc);
        }
    }
} // namespace xaml
