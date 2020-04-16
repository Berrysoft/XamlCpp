#include <xaml/meta/property_info.h>
#include <xaml/object.hpp>
#include <xaml/xaml_ptr.hpp>

struct xaml_property_info_impl : xaml_implement<xaml_property_info_impl, xaml_property_info, xaml_object>
{
private:
    xaml_ptr<xaml_string> m_name;

public:
    xaml_result XAML_CALL get_name(xaml_string**) const noexcept override;
    xaml_result XAML_CALL get_type(xaml_guid*) const noexcept override;
    xaml_result XAML_CALL get_can_read(bool*) const noexcept override;
    xaml_result XAML_CALL get_can_write(bool*) const noexcept override;
    xaml_result XAML_CALL get(xaml_object*, xaml_object**) const noexcept override;
    xaml_result XAML_CALL set(xaml_object*, xaml_object*) const noexcept override;
};
