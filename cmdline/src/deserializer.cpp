#include <xaml/cmdline/deserializer.h>

using namespace std;

xaml_result XAML_CALL xaml_cmdline_deserialize(xaml_type_info* type, xaml_cmdline_options* opts, xaml_object** ptr) noexcept
{
    xaml_ptr<xaml_object> result;
    XAML_RETURN_IF_FAILED(type->construct(&result));
    xaml_ptr<xaml_map_view> props;
    XAML_RETURN_IF_FAILED(opts->get_properties(&props));
    XAML_FOREACH_START(prop_pair, props);
    {
        xaml_ptr<xaml_key_value_pair> p;
        XAML_RETURN_IF_FAILED(prop_pair->query(&p));
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(p->get_value(&item));
        xaml_ptr<xaml_key_value_pair> pp;
        XAML_RETURN_IF_FAILED(item->query(&pp));
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(pp->get_key(&key));
        xaml_ptr<xaml_property_info> info;
        XAML_RETURN_IF_FAILED(key->query(&info));
        bool can_write;
        XAML_RETURN_IF_FAILED(info->get_can_write(&can_write));
        if (can_write)
        {
            xaml_ptr<xaml_object> value;
            XAML_RETURN_IF_FAILED(pp->get_value(&value));
            XAML_RETURN_IF_FAILED(info->set(result.get(), value.get()));
        }
    }
    XAML_FOREACH_END();
    xaml_ptr<xaml_map_view> cprops;
    XAML_RETURN_IF_FAILED(opts->get_collection_properties(&cprops));
    XAML_FOREACH_START(prop_pair, cprops);
    {
        xaml_ptr<xaml_key_value_pair> p;
        XAML_RETURN_IF_FAILED(prop_pair->query(&p));
        xaml_ptr<xaml_object> item;
        XAML_RETURN_IF_FAILED(p->get_value(&item));
        xaml_ptr<xaml_key_value_pair> pp;
        XAML_RETURN_IF_FAILED(item->query(&pp));
        xaml_ptr<xaml_object> key;
        XAML_RETURN_IF_FAILED(pp->get_key(&key));
        xaml_ptr<xaml_collection_property_info> prop;
        XAML_RETURN_IF_FAILED(key->query(&prop));
        bool can_add;
        XAML_RETURN_IF_FAILED(prop->get_can_add(&can_add));
        if (can_add)
        {
            xaml_ptr<xaml_object> value;
            XAML_RETURN_IF_FAILED(pp->get_value(&value));
            xaml_ptr<xaml_vector_view> values;
            XAML_RETURN_IF_FAILED(value->query(&values));
            XAML_FOREACH_START(v, values);
            {
                XAML_RETURN_IF_FAILED(prop->add(result.get(), v.get()));
            }
            XAML_FOREACH_END();
        }
    }
    XAML_FOREACH_END();
    return result->query(ptr);
}
