#include <helper.h>

xaml_result XAML_CALL xaml_delegate_2__xaml_object__xaml_vector_changed_args_1__xaml_string_new(
    xaml_result (*p)(xaml_object*, xaml_vector_changed_args<xaml_string>*) noexcept,
    xaml_delegate<xaml_object, xaml_vector_changed_args<xaml_string>>** ptr) noexcept
{
    return xaml_delegate_new<xaml_object, xaml_vector_changed_args<xaml_string>>(p, ptr);
}

xaml_result XAML_CALL xaml_observable_vector_1__xaml_string_new(xaml_observable_vector<xaml_string>** ptr) noexcept
{
    return xaml_observable_vector_new(ptr);
}

xaml_result XAML_CALL xaml_map_2__int32_t__xaml_object_new(xaml_map<int32_t, xaml_object>** ptr) noexcept
{
    return xaml_map_new(ptr);
}
