#include <xaml/deserializer.hpp>
#include <xaml/markup/markup_extension.hpp>
#include <xaml/parser.hpp>

using namespace std;

namespace xaml
{
    struct deserializer_markup_context : markup_context
    {
        weak_ptr<meta_class> current;
        weak_ptr<meta_class> current_element() const override { return current; }

        string_view prop;
        string_view current_property() const override { return prop; }

        map<string, weak_ptr<meta_class>>& symbols;
        weak_ptr<meta_class> find_element(string_view name) const override
        {
            if (name.empty())
                return current;
            else
            {
                auto it = symbols.find((string)name);
                if (it != symbols.end())
                    return it->second;
                else
                    return {};
            }
        }

        deserializer_markup_context(weak_ptr<meta_class> current, string_view prop, map<string, weak_ptr<meta_class>>& symbols)
            : current(current), prop(prop), symbols(symbols)
        {
        }

        ~deserializer_markup_context() override {}
    };

    shared_ptr<meta_class> deserializer::construct_impl(xaml_node& node, shared_ptr<meta_class> root)
    {
        auto t = node.type;
        if (node.map_class)
        {
            auto& [ns, name] = *node.map_class;
            auto map_type = m_ctx->get_type(ns, name);
            if (map_type) t = map_type;
        }
        shared_ptr<meta_class> c = t->construct();
        if (c)
        {
            deserialize_impl(c, node, root ? root : c);
            return c;
        }
        else
        {
            throw xaml_no_default_constructor(node.type);
        }
    }

    void deserializer::deserialize_impl(shared_ptr<meta_class> mc, xaml_node& node, shared_ptr<meta_class> root)
    {
        symbols.emplace(node.name, mc);
        for (auto& prop : node.properties)
        {
            auto& value = prop.value;
            switch (value.index())
            {
            case 0: // string
                prop.info->set(mc, box_value(get<string>(value)));
                break;
            case 2: // xaml_node
            {
                auto& cnode = get<xaml_node>(value);
                prop.info->set(mc, construct_impl(cnode, root));
                break;
            }
            }
        }
        for (auto& prop : node.collection_properties)
        {
            for (auto& n : prop.second.values)
            {
                prop.second.info->add(mc, construct_impl(n, root));
            }
        }
        for (auto& ev : node.events)
        {
            auto ref = m_ctx->get_type(root->get_type());
            auto [first, second] = ref->get_methods().equal_range(ev.value);
            if (first != second)
            {
                ev.info->add_this(mc, root, *first->second);
            }
            else
            {
                throw xaml_no_member(ref, ev.value);
            }
        }
    }

    void deserializer::deserialize_extensions(xaml_node& node)
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
                deserializer_markup_context context{ mc, prop.info->name(), symbols };
                auto ex = deserialize(n);
                ex->query<markup_extension>()->provide(*m_ctx, context);
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

    void deserializer::deserialize(xaml_node& node, shared_ptr<meta_class> mc)
    {
        if (mc)
        {
            deserialize_impl(mc, node, mc);
            deserialize_extensions(node);
        }
    }

    shared_ptr<meta_class> deserializer::deserialize(xaml_node& node)
    {
        auto c = construct_impl(node, nullptr);
        deserialize(node, c);
        return c;
    }

    shared_ptr<meta_class> deserializer::deserialize(markup_node& node)
    {
        shared_ptr<meta_class> ex = node.type->construct();
        for (auto& p : node.properties)
        {
            auto& value = p.value;
            switch (value.index())
            {
            case 0: // string
                p.info->set(ex, box_value(get<string>(value)));
                break;
            }
        }
        return ex;
    }
} // namespace xaml
