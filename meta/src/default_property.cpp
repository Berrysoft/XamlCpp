#include <xaml/meta/default_property.h>

using namespace std;

struct xaml_default_property_impl : xaml_implement<xaml_default_property_impl, xaml_default_property, xaml_object>
{
private:
    xaml_ptr<xaml_string> m_def_prop;

public:
    xaml_default_property_impl(xaml_ptr<xaml_string>&& name) : m_def_prop(move(name)) {}

    xaml_result XAML_CALL get_default_property(xaml_string** ptr) noexcept override
    {
        m_def_prop.add_ref_to(ptr);
        return XAML_S_OK;
    }
};

xaml_result xaml_default_property_new(xaml_string* name, xaml_default_property** ptr) noexcept
{
    return xaml_object_new<xaml_default_property_impl>(ptr, name);
}
