#ifndef XAML_BOX_H
#define XAML_BOX_H

#include <string.h>
#include <xaml/object.h>

XAML_CLASS(xaml_box, { 0x8ba3da41, 0x01bc, 0x4c97, { 0x92, 0x5e, 0xb1, 0x71, 0x57, 0xda, 0x5e, 0x31 } })

#ifdef __cplusplus
struct xaml_box : xaml_object
{
    virtual xaml_result XAML_CALL get_type(xaml_guid*) const noexcept = 0;
    virtual xaml_result XAML_CALL get_data(void const**) const noexcept = 0;
    virtual xaml_result XAML_CALL get_size(size_t*) const noexcept = 0;
    virtual xaml_result XAML_CALL set_data(xaml_guid XAML_CONST_REF, void const*, size_t) noexcept = 0;

    template <typename T>
    xaml_result set_value(T const& value)
    {
        return set_data(xaml_type_guid_v<T>, &value, sizeof(T));
    }

    template <typename T>
    xaml_result get_value(T& value)
    {
        xaml_guid type;
        XAML_RETURN_IF_FAILED(get_type(&type));
        if (type != xaml_type_guid_v<T>) return XAML_E_NOINTERFACE;
        void const* data;
        XAML_RETURN_IF_FAILED(get_data(&data));
        size_t size;
        XAML_RETURN_IF_FAILED(get_size(&size));
        memcpy(&value, data, size);
        return XAML_S_OK;
    }
};
#else
#define XAML_BOX_VTBL(type)                                            \
    xaml_result(XAML_CALL* get_type)(type const* const, xaml_guid*);   \
    xaml_result(XAML_CALL* get_data)(type const* const, void const**); \
    xaml_result(XAML_CALL* get_size)(type const* const, size_t*);      \
    xaml_result(XAML_CALL* set_data)(type* const, xaml_guid XAML_CONST_REF, void const*, size_t);

struct xaml_box
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_box)
        XAML_BOX_VTBL(xaml_box)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_box_new(xaml_guid XAML_CONST_REF, void const*, size_t, xaml_box**) XAML_NOEXCEPT;

XAML_TYPE(int, { 0x0ce68e8c, 0x51a4, 0x3115, { 0xbe, 0x13, 0xdd, 0x1a, 0xce, 0x42, 0xe6, 0x1f } })

#endif // !XAML_BOX_H
