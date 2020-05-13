#include <shared/filebox.hpp>
#include <xaml/ui/filebox.h>

xaml_result XAML_CALL xaml_open_filebox_new(xaml_open_filebox** ptr) noexcept
{
    return xaml_object_new<xaml_filebox_impl<xaml_open_filebox>>(ptr);
}

xaml_result XAML_CALL xaml_save_filebox_new(xaml_save_filebox** ptr) noexcept
{
    return xaml_object_new<xaml_filebox_impl<xaml_save_filebox>>(ptr);
}
