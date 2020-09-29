#include <xaml/box.h>
#include <xaml/object.h>

using namespace std;

#define __XAML_BOX_DEF_GEN(type)                                                                  \
    xaml_result XAML_CALL xaml_box_1__##type##_new(type value, xaml_box_1__##type** ptr) noexcept \
    {                                                                                             \
        return xaml_box_new<type>(value, ptr);                                                    \
    }

__XAML_BOX_DEF_GEN(xaml_bool)
__XAML_BOX_DEF_GEN(char)
__XAML_BOX_DEF_GEN(int8_t)
__XAML_BOX_DEF_GEN(int16_t)
__XAML_BOX_DEF_GEN(int32_t)
__XAML_BOX_DEF_GEN(int64_t)
__XAML_BOX_DEF_GEN(uint8_t)
__XAML_BOX_DEF_GEN(uint16_t)
__XAML_BOX_DEF_GEN(uint32_t)
__XAML_BOX_DEF_GEN(uint64_t)
__XAML_BOX_DEF_GEN(float)
__XAML_BOX_DEF_GEN(double)
__XAML_BOX_DEF_GEN(xaml_guid)
__XAML_BOX_DEF_GEN(xaml_version)
