#ifndef XAML_BOX_H
#define XAML_BOX_H

#ifdef __cplusplus
#include <cstring>
#include <xaml/xaml_ptr.hpp>
#else
#include <string.h>
#endif // __cplusplus

#include <xaml/object.h>
#include <xaml/string.h>

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

XAML_TYPE(bool, { 0xc3a0fdbf, 0xa30b, 0x315e, { 0xb0, 0x19, 0x42, 0xab, 0xac, 0xf7, 0x2c, 0xae } })
XAML_TYPE(xaml_char_t, { 0x2d08eb84, 0x64e6, 0x3688, { 0x80, 0xd7, 0xe0, 0xc5, 0x48, 0xac, 0x36, 0x2d } })
XAML_TYPE_NAME(signed char, schar, { 0xdc3c96bc, 0x48ce, 0x3ef7, { 0x8c, 0x15, 0x37, 0xbc, 0x7e, 0xc8, 0x07, 0xa1 } })
XAML_TYPE(short, { 0xe3e8f6e4, 0xe677, 0x3436, { 0x8e, 0x0e, 0x96, 0x3d, 0x3d, 0x6f, 0x81, 0x0a } })
XAML_TYPE(int, { 0x0ce68e8c, 0x51a4, 0x3115, { 0xbe, 0x13, 0xdd, 0x1a, 0xce, 0x42, 0xe6, 0x1f } })
XAML_TYPE(long, { 0x78c952b3, 0x23a5, 0x429f, { 0x84, 0xaf, 0x5c, 0xa9, 0x2b, 0x49, 0xf9, 0xfb } })
XAML_TYPE_NAME(long long, llong, { 0xf940ae92, 0xb495, 0x342e, { 0xab, 0x07, 0xe5, 0xff, 0x86, 0x3f, 0xb4, 0x15 } })
XAML_TYPE_NAME(unsigned char, uchar, { 0xb3b3b9f4, 0x12da, 0x3efd, { 0xb1, 0x31, 0xdf, 0x0e, 0x1f, 0x9f, 0x91, 0x4b } })
XAML_TYPE_NAME(unsigned short, ushort, { 0x33d9552a, 0xdc77, 0x3314, { 0x8e, 0xa3, 0x80, 0xbf, 0x05, 0x8e, 0xa1, 0x63 } })
XAML_TYPE_NAME(unsigned int, uint, { 0xe38cb0a1, 0x90ae, 0x3c05, { 0x9b, 0x2e, 0x06, 0x2a, 0x58, 0x4d, 0x6c, 0x50 } })
XAML_TYPE_NAME(unsigned long, ulong, { 0x089d52f5, 0xc09d, 0x4be7, { 0xa1, 0xce, 0x58, 0xf4, 0x54, 0x8b, 0xe8, 0x59 } })
XAML_TYPE_NAME(unsigned long long, ullong, { 0x5f5fc118, 0x3099, 0x32f3, { 0xab, 0xba, 0x75, 0x57, 0x06, 0xf8, 0xb7, 0xf8 } })
XAML_TYPE(float, { 0xa0782160, 0xfcb5, 0x30dc, { 0xb7, 0x5c, 0xb7, 0xc3, 0x04, 0xdf, 0xc3, 0xd3 } })
XAML_TYPE(double, { 0x9144b7d6, 0x3d5f, 0x3b29, { 0x81, 0x31, 0xff, 0x0d, 0xb5, 0x51, 0xe1, 0x7c } })
XAML_TYPE_NAME(long double, ldouble, { 0x3b0b7aa1, 0xe4c0, 0x3eab, { 0x8e, 0x85, 0xbc, 0xca, 0x45, 0xb7, 0x3a, 0xaf } })

#ifdef __cplusplus
template <typename T>
inline xaml_result xaml_box_new(T const& value, xaml_box** ptr) noexcept
{
    return xaml_box_new(xaml_type_guid_v<T>, &value, sizeof(T), ptr);
}

template <typename T, typename = void>
struct __box_impl
{
    xaml_result box(T const& value, xaml_object** ptr) const noexcept
    {
        return xaml_box_new(value, (xaml_box**)ptr);
    }

    xaml_result unbox(xaml_object* obj, T& value) const noexcept
    {
        xaml_box* box;
        XAML_RETURN_IF_FAILED(obj->query(&box));
        return box->get_value(value);
    }
};

template <typename T>
struct __box_impl<T*, std::enable_if_t<std::is_base_of_v<xaml_object, T>>>
{
    xaml_result box(T* value, xaml_object** ptr) const noexcept
    {
        return value->query(ptr);
    }

    xaml_result unbox(xaml_object* obj, T*& value) const noexcept
    {
        return obj->query(&value);
    }
};

template <typename T>
struct __box_impl<xaml_ptr<T>, std::enable_if_t<std::is_base_of_v<xaml_object, T>>>
{
    xaml_result box(xaml_ptr<T> const& value, xaml_object** ptr) const noexcept
    {
        return value->query(ptr);
    }

    xaml_result unbox(xaml_object* obj, xaml_ptr<T>& value) const noexcept
    {
        return obj->query(&value);
    }
};

template <typename T>
xaml_result box_value(T const& value, xaml_object** ptr) noexcept
{
    return __box_impl<T>{}.box(value, ptr);
}

template <typename T>
xaml_ptr<xaml_object> box_value(T const& value)
{
    xaml_ptr<xaml_object> obj;
    XAML_THROW_IF_FAILED(box_value(value, &obj));
    return obj;
}

template <typename T>
xaml_result unbox_value(xaml_object* obj, T& value) noexcept
{
    return __box_impl<T>{}.unbox(obj, value);
}

template <typename T>
T unbox_value(xaml_ptr<xaml_object> const& obj)
{
    T value;
    XAML_THROW_IF_FAILED(unbox_value(obj.get(), value));
    return value;
}
#endif // __cplusplus

#endif // !XAML_BOX_H
