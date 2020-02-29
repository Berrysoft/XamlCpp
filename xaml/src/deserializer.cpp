#include <xaml/deserializer.hpp>
#include <xaml/markup/markup_extension.hpp>
#include <xaml/parser.hpp>
#include <xaml/ui/control.hpp>

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
        weak_ptr<meta_class> data_context;
        weak_ptr<meta_class> find_element(string_view name) const override
        {
            if (name.empty())
                return data_context;
            else
            {
                auto it = symbols.find((string)name);
                if (it != symbols.end())
                    return it->second;
                else
                    return {};
            }
        }

        deserializer_markup_context(weak_ptr<meta_class> current, string_view prop, map<string, weak_ptr<meta_class>>& symbols, weak_ptr<meta_class> data_context)
            : current(current), prop(prop), symbols(symbols), data_context(data_context)
        {
        }

        ~deserializer_markup_context() override {}
    };

    shared_ptr<meta_class> deserializer::construct_impl(xaml_node& node, shared_ptr<meta_class> root)
    {
        auto t = node.type;
        if (node.map_class)
        {
            auto [ns, name] = *node.map_class;
            auto map_type = m_ctx->get_type(ns, name);
            if (map_type) t = map_type;
        }
        shared_ptr<meta_class> c{ t->construct() };
        if (c)
        {
            deserialize_impl(c, node, root ? root : c);
            return c;
        }
        else
        {
            throw xaml_no_default_constructor(node.type);
        }
        return nullptr;
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
                prop.info->set(mc.get(), get<string>(value));
                break;
            case 2: // xaml_node
            {
                auto& node = get<xaml_node>(value);
                prop.info->set(mc.get(), construct_impl(node, root));
                break;
            }
            }
        }
        for (auto& prop : node.collection_properties)
        {
            for (auto& n : prop.second.values)
            {
                prop.second.info->add(mc.get(), construct_impl(n, root));
            }
        }
        for (auto& ev : node.events)
        {
            auto ref = m_ctx->get_type(root->this_type());
            auto [first, second] = ref->get_methods().equal_range(ev.value);
            if (first != second)
            {
                ev.info->add_erased_this(mc.get(), root.get(), first->second.get());
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
                deserializer_markup_context context{ mc, prop.info->name(), symbols, static_pointer_cast<control>(mc.lock())->get_data_context() };
                auto ex = deserialize(n);
                static_pointer_cast<markup_extension>(ex)->provide(*m_ctx, context);
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
        shared_ptr<meta_class> ex{ node.type->construct() };
        for (auto& p : node.properties)
        {
            auto& value = p.value;
            switch (value.index())
            {
            case 0: // string
                p.info->set(ex.get(), get<string>(value));
                break;
            }
        }
        return ex;
    }
} // namespace xaml
