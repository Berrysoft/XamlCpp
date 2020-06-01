#define XAML_RAISE_LEVEL xaml_result_raise_warning

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

struct deserializer_impl
{
    xaml_ptr<xaml_meta_context> m_ctx;
    xaml_ptr<xaml_map> symbols;

    deserializer_impl(xaml_ptr<xaml_meta_context> const& ctx) noexcept : m_ctx(ctx) {}

    xaml_result init() noexcept
    {
        xaml_ptr<xaml_hasher> hasher;
        XAML_RETURN_IF_FAILED(xaml_hasher_string_default(&hasher));
        XAML_RETURN_IF_FAILED(xaml_map_new_with_hasher(hasher, &symbols));
        return XAML_S_OK;
    }

    xaml_result construct_impl(xaml_ptr<xaml_node> const& node, xaml_ptr<xaml_object> const& root, xaml_ptr<xaml_type_info> const& root_type, xaml_object** ptr) noexcept;

    xaml_result deserialize_impl(xaml_ptr<xaml_object> const& mc, xaml_ptr<xaml_node> const& node, xaml_ptr<xaml_object> const& root, xaml_ptr<xaml_type_info> const& root_type) noexcept;

    xaml_result deserialize_extensions(xaml_ptr<xaml_node> const& node) noexcept;

    xaml_result deserialize(xaml_ptr<xaml_node> const& node, xaml_ptr<xaml_object> const& mc, xaml_ptr<xaml_type_info> const& root_type) noexcept;

    xaml_result deserialize(xaml_ptr<xaml_node> const& node, xaml_object** ptr) noexcept;

    xaml_result deserialize(xaml_ptr<xaml_markup_node> const& node, xaml_markup_extension** ptr) noexcept;
};

xaml_result deserializer_impl::construct_impl(xaml_ptr<xaml_node> const& node, xaml_ptr<xaml_object> const& root, xaml_ptr<xaml_type_info> const& root_type, xaml_object** ptr) noexcept
{
    xaml_ptr<xaml_type_info> t;
    XAML_RETURN_IF_FAILED(node->get_type(&t));
    xaml_ptr<xaml_string> map_class;
    XAML_RETURN_IF_FAILED(node->get_map_class(&map_class));
    if (map_class)
    {
        xaml_ptr<xaml_reflection_info> map_type;
        if (XAML_SUCCEEDED(m_ctx->get_type_by_name(map_class, &map_type)))
        {
            t = nullptr;
            XAML_RETURN_IF_FAILED(map_type->query(&t));
        }
    }
    xaml_ptr<xaml_object> c;
    XAML_RETURN_IF_FAILED(t->construct(&c));
    XAML_RETURN_IF_FAILED(deserialize_impl(c, node, root ? root : c, root_type));
    return c->query(ptr);
}

xaml_result deserializer_impl::deserialize_impl(xaml_ptr<xaml_object> const& mc, xaml_ptr<xaml_node> const& node, xaml_ptr<xaml_object> const& root, xaml_ptr<xaml_type_info> const& root_type) noexcept
{
    {
        xaml_ptr<xaml_string> node_name;
        XAML_RETURN_IF_FAILED(node->get_name(&node_name));
        bool replaced;
        XAML_RETURN_IF_FAILED(symbols->insert(node_name, mc, &replaced));
    }
    {
        xaml_ptr<xaml_vector> node_properties;
        XAML_RETURN_IF_FAILED(node->get_properties(&node_properties));
        XAML_FOREACH_START(prop_item, node_properties);
        {
            xaml_ptr<xaml_attribute_property> prop;
            XAML_RETURN_IF_FAILED(prop_item->query(&prop));
            xaml_ptr<xaml_node_base> value;
            XAML_RETURN_IF_FAILED(prop->get_value(&value));
            xaml_ptr<xaml_property_info> info;
            XAML_RETURN_IF_FAILED(prop->get_info(&info));
            if (auto s = value.query<xaml_string_node>())
            {
                xaml_ptr<xaml_string> str;
                XAML_RETURN_IF_FAILED(s->get_value(&str));
                xaml_guid type;
                XAML_RETURN_IF_FAILED(info->get_type(&type));
                xaml_ptr<xaml_reflection_info> type_info;
                if (XAML_SUCCEEDED(m_ctx->get_type(type, &type_info)))
                {
                    if (auto enum_info = type_info.query<xaml_enum_info>())
                    {
                        xaml_ptr<xaml_box> box;
                        XAML_RETURN_IF_FAILED(enum_info->get_value(str, &box));
                        XAML_RETURN_IF_FAILED(info->set(mc, box));
                    }
                    else
                    {
                        XAML_RETURN_IF_FAILED(info->set(mc, str));
                    }
                }
                else
                {
                    XAML_RETURN_IF_FAILED(info->set(mc, str));
                }
            }
            else if (auto cnode = value.query<xaml_node>())
            {
                xaml_ptr<xaml_object> c;
                XAML_RETURN_IF_FAILED(construct_impl(cnode, root, root_type, &c));
                XAML_RETURN_IF_FAILED(info->set(mc, c));
            }
        }
        XAML_FOREACH_END();
    }
    {
        xaml_ptr<xaml_map> node_cprops;
        XAML_RETURN_IF_FAILED(node->get_collection_properties(&node_cprops));
        XAML_FOREACH_START(prop_item, node_cprops);
        {
            xaml_ptr<xaml_key_value_pair> prop;
            XAML_RETURN_IF_FAILED(prop_item->query(&prop));
            xaml_ptr<xaml_object> value;
            XAML_RETURN_IF_FAILED(prop->get_value(&value));
            xaml_ptr<xaml_attribute_collection_property> cp;
            XAML_RETURN_IF_FAILED(value->query(&cp));
            xaml_ptr<xaml_vector> values;
            XAML_RETURN_IF_FAILED(cp->get_values(&values));
            xaml_ptr<xaml_collection_property_info> info;
            XAML_RETURN_IF_FAILED(cp->get_info(&info));
            XAML_FOREACH_START(n_item, values);
            {
                xaml_ptr<xaml_node> n;
                XAML_RETURN_IF_FAILED(n_item->query(&n));
                xaml_ptr<xaml_object> c;
                XAML_RETURN_IF_FAILED(construct_impl(n, root, root_type, &c));
                XAML_RETURN_IF_FAILED(info->add(mc, c));
            }
            XAML_FOREACH_END();
        }
        XAML_FOREACH_END();
    }
    {
        xaml_ptr<xaml_vector> node_events;
        XAML_RETURN_IF_FAILED(node->get_events(&node_events));
        XAML_FOREACH_START(ev_item, node_events);
        {
            xaml_ptr<xaml_attribute_event> ev;
            XAML_RETURN_IF_FAILED(ev_item->query(&ev));
            xaml_ptr<xaml_string> ev_value;
            XAML_RETURN_IF_FAILED(ev->get_value(&ev_value));
            xaml_ptr<xaml_method_info> method;
            XAML_RETURN_IF_FAILED(root_type->get_method(ev_value, &method));
            xaml_ptr<xaml_vector_view> bind_args;
            XAML_RETURN_IF_FAILED(xaml_delegate_pack_args(&bind_args, root));
            xaml_ptr<xaml_delegate> binded_method;
            XAML_RETURN_IF_FAILED(xaml_delegate_bind(method, bind_args, &binded_method));
            xaml_ptr<xaml_event_info> info;
            XAML_RETURN_IF_FAILED(ev->get_info(&info));
            int32_t token;
            XAML_RETURN_IF_FAILED(info->add(mc, binded_method, &token));
        }
        XAML_FOREACH_END();
    }
    return XAML_S_OK;
}

xaml_result deserializer_impl::deserialize_extensions(xaml_ptr<xaml_node> const& node) noexcept
{
    xaml_ptr<xaml_string> node_name;
    XAML_RETURN_IF_FAILED(node->get_name(&node_name));
    xaml_ptr<xaml_object> mc;
    XAML_RETURN_IF_FAILED(symbols->lookup(node_name, &mc));
    {
        xaml_ptr<xaml_vector> node_properties;
        XAML_RETURN_IF_FAILED(node->get_properties(&node_properties));
        XAML_FOREACH_START(prop_item, node_properties);
        {
            xaml_ptr<xaml_attribute_property> prop;
            XAML_RETURN_IF_FAILED(prop_item->query(&prop));
            xaml_ptr<xaml_node_base> value;
            XAML_RETURN_IF_FAILED(prop->get_value(&value));
            xaml_ptr<xaml_property_info> info;
            XAML_RETURN_IF_FAILED(prop->get_info(&info));
            if (auto n = value.query<xaml_markup_node>())
            {
                xaml_ptr<xaml_string> info_name;
                XAML_RETURN_IF_FAILED(info->get_name(&info_name));
                xaml_ptr<xaml_markup_context> context;
                XAML_RETURN_IF_FAILED(xaml_object_new<xaml_deserializer_context_impl>(&context, mc, info_name, symbols));
                xaml_ptr<xaml_markup_extension> ex;
                XAML_RETURN_IF_FAILED(deserialize(n, &ex));
                XAML_RETURN_IF_FAILED(ex->provide(m_ctx, context));
            }
            else if (auto cnode = value.query<xaml_node>())
            {
                XAML_RETURN_IF_FAILED(deserialize_extensions(cnode));
            }
        }
        XAML_FOREACH_END();
    }
    {
        xaml_ptr<xaml_map> node_cprops;
        XAML_RETURN_IF_FAILED(node->get_collection_properties(&node_cprops));
        XAML_FOREACH_START(prop_item, node_cprops);
        {
            xaml_ptr<xaml_key_value_pair> prop;
            XAML_RETURN_IF_FAILED(prop_item->query(&prop));
            xaml_ptr<xaml_object> value;
            XAML_RETURN_IF_FAILED(prop->get_value(&value));
            xaml_ptr<xaml_attribute_collection_property> cp;
            XAML_RETURN_IF_FAILED(value->query(&cp));
            xaml_ptr<xaml_vector> values;
            XAML_RETURN_IF_FAILED(cp->get_values(&values));
            XAML_FOREACH_START(n_item, values);
            {
                xaml_ptr<xaml_node> n;
                XAML_RETURN_IF_FAILED(n_item->query(&n));
                XAML_RETURN_IF_FAILED(deserialize_extensions(n));
            }
            XAML_FOREACH_END();
        }
        XAML_FOREACH_END();
    }
    return XAML_S_OK;
}

xaml_result deserializer_impl::deserialize(xaml_ptr<xaml_node> const& node, xaml_ptr<xaml_object> const& mc, xaml_ptr<xaml_type_info> const& t) noexcept
{
    if (mc)
    {
        XAML_RETURN_IF_FAILED(deserialize_impl(mc, node, mc, t));
        XAML_RETURN_IF_FAILED(deserialize_extensions(node));
    }
    return XAML_S_OK;
}

xaml_result deserializer_impl::deserialize(xaml_ptr<xaml_node> const& node, xaml_object** ptr) noexcept
{
    xaml_ptr<xaml_type_info> t;
    XAML_RETURN_IF_FAILED(node->get_type(&t));
    xaml_ptr<xaml_object> c;
    XAML_RETURN_IF_FAILED(construct_impl(node, nullptr, t, &c));
    XAML_RETURN_IF_FAILED(deserialize(node, c, t));
    return c->query(ptr);
}

xaml_result deserializer_impl::deserialize(xaml_ptr<xaml_markup_node> const& node, xaml_markup_extension** ptr) noexcept
{
    xaml_ptr<xaml_type_info> type;
    XAML_RETURN_IF_FAILED(node->get_type(&type));
    xaml_ptr<xaml_object> ex;
    XAML_RETURN_IF_FAILED(type->construct(&ex));
    {
        xaml_ptr<xaml_vector> node_properties;
        XAML_RETURN_IF_FAILED(node->get_properties(&node_properties));
        XAML_FOREACH_START(prop_item, node_properties);
        {
            xaml_ptr<xaml_attribute_property> prop;
            XAML_RETURN_IF_FAILED(prop_item->query(&prop));
            xaml_ptr<xaml_node_base> value;
            XAML_RETURN_IF_FAILED(prop->get_value(&value));
            xaml_ptr<xaml_property_info> info;
            XAML_RETURN_IF_FAILED(prop->get_info(&info));
            if (auto s = value.query<xaml_string_node>())
            {
                xaml_ptr<xaml_string> str;
                XAML_RETURN_IF_FAILED(s->get_value(&str));
                xaml_guid t;
                XAML_RETURN_IF_FAILED(info->get_type(&t));
                xaml_ptr<xaml_reflection_info> type_info;
                if (XAML_SUCCEEDED(m_ctx->get_type(t, &type_info)))
                {
                    if (auto enum_info = type_info.query<xaml_enum_info>())
                    {
                        xaml_ptr<xaml_box> box;
                        XAML_RETURN_IF_FAILED(enum_info->get_value(str, &box));
                        XAML_RETURN_IF_FAILED(info->set(ex, box));
                    }
                    else
                    {
                        XAML_RETURN_IF_FAILED(info->set(ex, str));
                    }
                }
                else
                {
                    XAML_RETURN_IF_FAILED(info->set(ex, str));
                }
            }
            else if (auto n = value.query<xaml_markup_node>())
            {
                xaml_ptr<xaml_string> info_name;
                XAML_RETURN_IF_FAILED(info->get_name(&info_name));
                xaml_ptr<xaml_markup_context> context;
                XAML_RETURN_IF_FAILED(xaml_object_new<xaml_deserializer_context_impl>(&context, ex, info_name, symbols));
                xaml_ptr<xaml_markup_extension> obj;
                XAML_RETURN_IF_FAILED(deserialize(n, &obj));
                XAML_RETURN_IF_FAILED(obj->provide(m_ctx, context));
            }
        }
        XAML_FOREACH_END();
    }
    return ex->query(ptr);
}

xaml_result XAML_CALL xaml_parser_deserialize(xaml_meta_context* ctx, xaml_node* node, xaml_object** ptr) noexcept
{
    deserializer_impl des{ ctx };
    XAML_RETURN_IF_FAILED(des.init());
    return des.deserialize(node, ptr);
}

xaml_result XAML_CALL xaml_parser_deserialize_inplace(xaml_meta_context* ctx, xaml_node* node, xaml_object* mc) noexcept
{
    deserializer_impl des{ ctx };
    XAML_RETURN_IF_FAILED(des.init());
    xaml_guid type;
    XAML_RETURN_IF_FAILED(mc->get_guid(&type));
    xaml_ptr<xaml_reflection_info> info;
    XAML_RETURN_IF_FAILED(ctx->get_type(type, &info));
    xaml_ptr<xaml_type_info> t;
    XAML_RETURN_IF_FAILED(info->query(&t));
    return des.deserialize(node, mc, t);
}

xaml_result XAML_CALL xaml_parser_deserialize_markup(xaml_meta_context* ctx, xaml_markup_node* node, xaml_markup_extension** ptr) noexcept
{
    deserializer_impl des{ ctx };
    XAML_RETURN_IF_FAILED(des.init());
    return des.deserialize(node, ptr);
}
