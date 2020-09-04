#include <xaml/meta/default_property.h>
#include <xaml/meta/meta_macros.h>

using namespace std;

struct xaml_default_property_impl : xaml_implement<xaml_default_property_impl, xaml_default_property>
{
private:
    xaml_ptr<xaml_string> m_def_prop;

public:
    xaml_default_property_impl(xaml_ptr<xaml_string>&& name) noexcept : m_def_prop(move(name)) {}

    xaml_result XAML_CALL get_default_property(xaml_string** ptr) noexcept override
    {
        return m_def_prop->query(ptr);
    }
};

xaml_result XAML_CALL xaml_default_property_new(xaml_string* name, xaml_default_property** ptr) noexcept
{
    return xaml_object_new<xaml_default_property_impl>(ptr, name);
}

xaml_result XAML_CALL xaml_default_property_register(xaml_meta_context* ctx) noexcept
{
    XAML_TYPE_INFO_NEW(xaml_default_property, "xaml/meta/default_property.h");
    XAML_TYPE_INFO_ADD_PROP_RD(default_property, xaml_string);
    return ctx->add_type(__info);
}
