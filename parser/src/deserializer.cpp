#include <xaml/markup/markup_extension.h>
#include <xaml/parser/deserializer.h>
#include <xaml/parser/parser.h>

using namespace std;

struct xaml_deserializer_context_impl : xaml_implement<xaml_deserializer_context_impl, xaml_markup_context, xaml_object>
{
    xaml_object* m_current;
    xaml_result XAML_CALL get_current_element(xaml_object** ptr) noexcept override
    {
        return m_current->query(ptr);
    }

    xaml_ptr<xaml_string> m_prop;
    xaml_result XAML_CALL get_current_property(xaml_string** ptr) noexcept override
    {
        return m_prop->query(ptr);
    }

    xaml_ptr<xaml_map> m_symbols;
    xaml_result XAML_CALL find_element(xaml_string* key, xaml_object** ptr) noexcept override
    {
        return m_symbols->lookup(key, ptr);
    }

    xaml_deserializer_context_impl(xaml_object* current, xaml_ptr<xaml_string> const& prop, xaml_ptr<xaml_map> const& symbols) noexcept
        : m_current(current), m_prop(prop), m_symbols(symbols) {}
};

//
//    shared_ptr<meta_class> deserializer::construct_impl(xaml_node& node, shared_ptr<meta_class> root)
//    {
//        auto t = node.type;
//        if (node.map_class)
//        {
//            auto& [ns, name] = *node.map_class;
//            auto map_type = m_ctx->get_type(ns, name);
//            if (map_type) t = map_type;
//        }
//        shared_ptr<meta_class> c = t->construct();
//        if (c)
//        {
//            deserialize_impl(c, node, root ? root : c);
//            return c;
//        }
//        else
//        {
//            throw xaml_no_default_constructor(node.type);
//        }
//    }
//
//    void deserializer::deserialize_impl(shared_ptr<meta_class> mc, xaml_node& node, shared_ptr<meta_class> root)
//    {
//        symbols.emplace(node.name, mc);
//        for (auto& prop : node.properties)
//        {
//            auto& value = prop.value;
//            switch (value.index())
//            {
//            case 0: // string
//                prop.info->set(mc, box_value(get<string>(value)));
//                break;
//            case 2: // xaml_node
//            {
//                auto& cnode = get<xaml_node>(value);
//                prop.info->set(mc, construct_impl(cnode, root));
//                break;
//            }
//            }
//        }
//        for (auto& prop : node.collection_properties)
//        {
//            for (auto& n : prop.second.values)
//            {
//                prop.second.info->add(mc, construct_impl(n, root));
//            }
//        }
//        for (auto& ev : node.events)
//        {
//            auto ref = m_ctx->get_type(root->get_type());
//            auto [first, second] = ref->get_methods().equal_range(ev.value);
//            if (first != second)
//            {
//                ev.info->add_this(mc, root, *first->second);
//            }
//            else
//            {
//                throw xaml_no_member(ref, ev.value);
//            }
//        }
//    }
//
//    void deserializer::deserialize_extensions(xaml_node& node)
//    {
//        auto mc = symbols[node.name];
//        for (auto& prop : node.properties)
//        {
//            auto& value = prop.value;
//            switch (value.index())
//            {
//            case 1: // markup_node
//            {
//                auto& n = get<markup_node>(value);
//                deserializer_markup_context context{ mc, prop.info->name(), symbols };
//                auto ex = deserialize(n);
//                ex->query<markup_extension>()->provide(*m_ctx, context);
//                break;
//            }
//            case 2: // xaml_node
//                deserialize_extensions(get<xaml_node>(value));
//                break;
//            }
//        }
//        for (auto& prop : node.collection_properties)
//        {
//            for (auto& n : prop.second.values)
//            {
//                deserialize_extensions(n);
//            }
//        }
//    }
//
//    void deserializer::deserialize(xaml_node& node, shared_ptr<meta_class> mc)
//    {
//        if (mc)
//        {
//            deserialize_impl(mc, node, mc);
//            deserialize_extensions(node);
//        }
//    }
//
//    shared_ptr<meta_class> deserializer::deserialize(xaml_node& node)
//    {
//        auto c = construct_impl(node, nullptr);
//        deserialize(node, c);
//        return c;
//    }
//
//    shared_ptr<meta_class> deserializer::deserialize(markup_node& node)
//    {
//        shared_ptr<meta_class> ex = node.type->construct();
//        for (auto& p : node.properties)
//        {
//            auto& value = p.value;
//            switch (value.index())
//            {
//            case 0: // string
//                p.info->set(ex, box_value(get<string>(value)));
//                break;
//            }
//        }
//        return ex;
//    }
