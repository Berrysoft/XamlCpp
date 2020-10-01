#include <xaml/cmdline/deserializer.h>

using namespace std;

xaml_result XAML_CALL xaml_cmdline_deserialize(xaml_type_info* type, xaml_cmdline_options* opts, xaml_object** ptr) noexcept
{
    xaml_ptr<xaml_object> result;
    XAML_RETURN_IF_FAILED(type->construct(&result));
    xaml_ptr<xaml_map<xaml_property_info, xaml_string>> props;
    XAML_RETURN_IF_FAILED(opts->get_properties(&props));
    XAML_FOREACH_START(xaml_key_value_pair_2__xaml_property_info__xaml_string, p, props);
    {
        xaml_ptr<xaml_property_info> info;
        XAML_RETURN_IF_FAILED(p->get_key(&info));
        bool can_write;
        XAML_RETURN_IF_FAILED(info->get_can_write(&can_write));
        if (can_write)
        {
            xaml_ptr<xaml_string> value;
            XAML_RETURN_IF_FAILED(p->get_value(&value));
            XAML_RETURN_IF_FAILED(info->set(result, value));
        }
    }
    XAML_FOREACH_END();
    xaml_ptr<xaml_map<xaml_string, xaml_key_value_pair<xaml_collection_property_info, xaml_vector<xaml_string>>>> cprops;
    XAML_RETURN_IF_FAILED(opts->get_collection_properties(&cprops));
    XAML_FOREACH_START(xaml_key_value_pair_2__xaml_string__xaml_key_value_pair_2__xaml_collection_property_info__xaml_vector_1__xaml_string, p, cprops);
    {
        xaml_ptr<xaml_key_value_pair<xaml_collection_property_info, xaml_vector<xaml_string>>> pp;
        XAML_RETURN_IF_FAILED(p->get_value(&pp));
        xaml_ptr<xaml_collection_property_info> prop;
        XAML_RETURN_IF_FAILED(pp->get_key(&prop));
        bool can_add;
        XAML_RETURN_IF_FAILED(prop->get_can_add(&can_add));
        if (can_add)
        {
            xaml_ptr<xaml_vector<xaml_string>> values;
            XAML_RETURN_IF_FAILED(pp->get_value(&values));
            XAML_FOREACH_START(xaml_string, v, values);
            {
                XAML_RETURN_IF_FAILED(prop->add(result, v));
            }
            XAML_FOREACH_END();
        }
    }
    XAML_FOREACH_END();
    return result->query(ptr);
}
