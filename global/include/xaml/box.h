#ifndef XAML_BOX_H
#define XAML_BOX_H

#ifdef __cplusplus
    #include <xaml/ptr.hpp>
#endif // __cplusplus

#include <xaml/object.h>
#include <xaml/string.h>
#include <xaml/version.h>

XAML_TYPE_BASE(xaml_box_1, { 0x9a9177c7, 0xcf5f, 0x31ab, { 0x84, 0x95, 0x96, 0xf5, 0x8a, 0xc5, 0xdf, 0x3a } })

#define XAML_BOX_1_VTBL(type, TN, TI)          \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(get_value, type, TI*);         \
    XAML_METHOD(set_value, type, TI XAML_CONST_REF)

#ifdef __cplusplus
XAML_DECL_INTERFACE_T_(xaml_box, xaml_object, XAML_BOX_1_VTBL)

    #define XAML_BOX_1_NAME(type) xaml_box<type>

    #define __XAML_BOX_1_TYPE(type) typedef xaml_box<type> xaml_box_1__##type;
#else
    #define XAML_BOX_1_NAME(type) xaml_box_1__##type

    #define __XAML_BOX_1_TYPE(type_name, type_interface) \
        XAML_DECL_INTERFACE_T_(xaml_box_1, type_name, XAML_BOX_1_VTBL, type_name, type_interface)
#endif // __cplusplus
#define XAML_BOX_1_TYPE(type) __XAML_BOX_1_TYPE(type)

#ifdef __cplusplus
template <typename T>
struct __xaml_box_implement : xaml_implement<__xaml_box_implement<T>, xaml_box<T>>
{
    T m_value;

    __xaml_box_implement(T const& value) noexcept : m_value(value) {}

    xaml_result XAML_CALL get_value(T* ptr) noexcept override
    {
        *ptr = m_value;
        return XAML_S_OK;
    }

    xaml_result XAML_CALL set_value(T const& value) noexcept override
    {
        m_value = value;
        return XAML_S_OK;
    }
};

template <typename T>
xaml_result XAML_CALL xaml_box_new(T const& value, xaml_box<T>** ptr) noexcept
{
    return xaml_object_new<__xaml_box_implement<T>>(ptr, value);
}
#endif // __cplusplus

XAML_TYPE(xaml_bool, { 0xc3a0fdbf, 0xa30b, 0x315e, { 0xb0, 0x19, 0x42, 0xab, 0xac, 0xf7, 0x2c, 0xae } })
XAML_TYPE(char, { 0x2d08eb84, 0x64e6, 0x3688, { 0x80, 0xd7, 0xe0, 0xc5, 0x48, 0xac, 0x36, 0x2d } })
XAML_TYPE_NAME(XAML_STD int8_t, int8, { 0xdc3c96bc, 0x48ce, 0x3ef7, { 0x8c, 0x15, 0x37, 0xbc, 0x7e, 0xc8, 0x07, 0xa1 } })
XAML_TYPE_NAME(XAML_STD int16_t, int16, { 0xe3e8f6e4, 0xe677, 0x3436, { 0x8e, 0x0e, 0x96, 0x3d, 0x3d, 0x6f, 0x81, 0x0a } })
XAML_TYPE_NAME(XAML_STD int32_t, int32, { 0x0ce68e8c, 0x51a4, 0x3115, { 0xbe, 0x13, 0xdd, 0x1a, 0xce, 0x42, 0xe6, 0x1f } })
XAML_TYPE_NAME(XAML_STD int64_t, int64, { 0xf940ae92, 0xb495, 0x342e, { 0xab, 0x07, 0xe5, 0xff, 0x86, 0x3f, 0xb4, 0x15 } })
XAML_TYPE_NAME(XAML_STD uint8_t, uint8, { 0xb3b3b9f4, 0x12da, 0x3efd, { 0xb1, 0x31, 0xdf, 0x0e, 0x1f, 0x9f, 0x91, 0x4b } })
XAML_TYPE_NAME(XAML_STD uint16_t, uint16, { 0x33d9552a, 0xdc77, 0x3314, { 0x8e, 0xa3, 0x80, 0xbf, 0x05, 0x8e, 0xa1, 0x63 } })
XAML_TYPE_NAME(XAML_STD uint32_t, uint32, { 0xe38cb0a1, 0x90ae, 0x3c05, { 0x9b, 0x2e, 0x06, 0x2a, 0x58, 0x4d, 0x6c, 0x50 } })
XAML_TYPE_NAME(XAML_STD uint64_t, uint64, { 0x5f5fc118, 0x3099, 0x32f3, { 0xab, 0xba, 0x75, 0x57, 0x06, 0xf8, 0xb7, 0xf8 } })
XAML_TYPE(float, { 0xa0782160, 0xfcb5, 0x30dc, { 0xb7, 0x5c, 0xb7, 0xc3, 0x04, 0xdf, 0xc3, 0xd3 } })
XAML_TYPE(double, { 0x9144b7d6, 0x3d5f, 0x3b29, { 0x81, 0x31, 0xff, 0x0d, 0xb5, 0x51, 0xe1, 0x7c } })
XAML_TYPE(xaml_guid, { 0x3c88cf27, 0x75ef, 0x3412, { 0x86, 0x88, 0x70, 0x59, 0xee, 0xb2, 0x65, 0x4c } })
XAML_TYPE(xaml_version, { 0x9fd1fbb5, 0xc8a0, 0x3ad2, { 0x90, 0x47, 0xfb, 0xfd, 0x22, 0x21, 0x54, 0x9b } })

#define __XAML_BOX_DECL_GEN(type) \
    EXTERN_C XAML_API xaml_result XAML_CALL xaml_box_1__##type##_new(type, xaml_box_1__##type** ptr) XAML_NOEXCEPT;

#ifndef xaml_box_1__xaml_bool_defined
    #define xaml_box_1__xaml_bool_defined
XAML_BOX_1_TYPE(XAML_T_V(xaml_bool))
#endif // !xaml_box_1__xaml_bool_defined
__XAML_BOX_DECL_GEN(xaml_bool)

#ifndef xaml_box_1__char_defined
    #define xaml_box_1__char_defined
XAML_BOX_1_TYPE(XAML_T_V(char))
#endif // !xaml_box_1__char_defined
__XAML_BOX_DECL_GEN(char)

#ifndef xaml_box_1__int8_t_defined
    #define xaml_box_1__int8_t_defined
XAML_BOX_1_TYPE(XAML_T_V(int8_t))
#endif // !xaml_box_1__int8_t_defined
__XAML_BOX_DECL_GEN(int8_t)

#ifndef xaml_box_1__int16_t_defined
    #define xaml_box_1__int16_t_defined
XAML_BOX_1_TYPE(XAML_T_V(int16_t))
#endif // !xaml_box_1__int16_t_defined
__XAML_BOX_DECL_GEN(int16_t)

#ifndef xaml_box_1__int32_t_defined
    #define xaml_box_1__int32_t_defined
XAML_BOX_1_TYPE(XAML_T_V(int32_t))
#endif // !xaml_box_1__int32_t_defined
__XAML_BOX_DECL_GEN(int32_t)

#ifndef xaml_box_1__int64_t_defined
    #define xaml_box_1__int64_t_defined
XAML_BOX_1_TYPE(XAML_T_V(int64_t))
#endif // !xaml_box_1__int64_t_defined
__XAML_BOX_DECL_GEN(int64_t)

#ifndef xaml_box_1__uint8_t_defined
    #define xaml_box_1__uint8_t_defined
XAML_BOX_1_TYPE(XAML_T_V(uint8_t))
#endif // !xaml_box_1__uint8_t_defined
__XAML_BOX_DECL_GEN(uint8_t)

#ifndef xaml_box_1__uint16_t_defined
    #define xaml_box_1__uint16_t_defined
XAML_BOX_1_TYPE(XAML_T_V(uint16_t))
#endif // !xaml_box_1__uint16_t_defined
__XAML_BOX_DECL_GEN(uint16_t)

#ifndef xaml_box_1__uint32_t_defined
    #define xaml_box_1__uint32_t_defined
XAML_BOX_1_TYPE(XAML_T_V(uint32_t))
#endif // !xaml_box_1__uint32_t_defined
__XAML_BOX_DECL_GEN(uint32_t)

#ifndef xaml_box_1__uint64_t_defined
    #define xaml_box_1__uint64_t_defined
XAML_BOX_1_TYPE(XAML_T_V(uint64_t))
#endif // !xaml_box_1__uint64_t_defined
__XAML_BOX_DECL_GEN(uint64_t)

#ifndef xaml_box_1__float_defined
    #define xaml_box_1__float_defined
XAML_BOX_1_TYPE(XAML_T_V(float))
#endif // !xaml_box_1__float_defined
__XAML_BOX_DECL_GEN(float)

#ifndef xaml_box_1__double_defined
    #define xaml_box_1__double_defined
XAML_BOX_1_TYPE(XAML_T_V(double))
#endif // !xaml_box_1__double_defined
__XAML_BOX_DECL_GEN(double)

#ifndef xaml_box_1__xaml_guid_defined
    #define xaml_box_1__xaml_guid_defined
XAML_BOX_1_TYPE(XAML_T_V(xaml_guid))
#endif // !xaml_box_1__xaml_guid_defined
__XAML_BOX_DECL_GEN(xaml_guid)

#ifndef xaml_box_1__xaml_version_defined
    #define xaml_box_1__xaml_version_defined
XAML_BOX_1_TYPE(XAML_T_V(xaml_version))
#endif // !xaml_box_1__xaml_version_defined
__XAML_BOX_DECL_GEN(xaml_version)

#undef __XAML_BOX_DECL_GEN

#ifdef __cplusplus
template <typename T, typename = void>
struct __xaml_box_impl
{
    using boxed_type = xaml_box<T>;

    xaml_result operator()(T const& value, xaml_box<T>** ptr) const noexcept
    {
        return xaml_box_new(value, ptr);
    }
};

template <typename T, typename = void>
struct __xaml_unbox_impl
{
    xaml_result operator()(xaml_ptr<xaml_object> const& obj, T* value) const noexcept
    {
        xaml_ptr<xaml_box<T>> box;
        XAML_RETURN_IF_FAILED(obj->query(&box));
        return box->get_value(value);
    }
};

template <typename T>
struct __xaml_box_impl<T*, std::enable_if_t<std::is_base_of_v<xaml_object, T>>>
{
    using boxed_type = std::conditional_t<xaml_type_has_guid_v<T>, T, xaml_object>;

    xaml_result operator()(T* value, boxed_type** ptr) const noexcept
    {
        if (value)
        {
            return value->xaml_object::query(ptr);
        }
        else
        {
            *ptr = nullptr;
            return XAML_S_OK;
        }
    }
};

template <typename T>
struct __xaml_unbox_impl<T*, std::enable_if_t<std::is_base_of_v<xaml_object, T>>>
{
    xaml_result operator()(xaml_ptr<xaml_object> const& obj, T** value) const noexcept
    {
        if (obj)
        {
            return obj->query(value);
        }
        else
        {
            *value = nullptr;
            return XAML_S_OK;
        }
    }
};

template <typename T>
struct __xaml_box_impl<xaml_ptr<T>, std::enable_if_t<std::is_base_of_v<xaml_object, T>>>
{
    using boxed_type = std::conditional_t<xaml_type_has_guid_v<T>, T, xaml_object>;

    xaml_result operator()(xaml_ptr<T> const& value, boxed_type** ptr) const noexcept
    {
        if (value)
        {
            return value->xaml_object::query(ptr);
        }
        else
        {
            *ptr = nullptr;
            return XAML_S_OK;
        }
    }
};

template <typename T>
struct __xaml_box_impl<T, std::enable_if_t<std::is_convertible_v<T, std::string_view>>>
{
    using boxed_type = xaml_string;

    xaml_result operator()(std::string_view value, xaml_string** ptr) const noexcept
    {
        xaml_ptr<xaml_string> str;
        XAML_RETURN_IF_FAILED(xaml_string_new(value, &str));
        return str->query(ptr);
    }
};

template <typename T>
struct __xaml_unbox_impl<T, std::enable_if_t<std::is_convertible_v<std::string_view, T>>>
{
    xaml_result operator()(xaml_ptr<xaml_object> const& obj, T* value) const noexcept
    {
        xaml_ptr<xaml_string> str;
        XAML_RETURN_IF_FAILED(obj->query(&str));
        std::string_view view;
        XAML_RETURN_IF_FAILED(to_string_view(str, &view));
        *value = view;
        return XAML_S_OK;
    }
};

template <typename T>
using __xaml_boxed_t = typename __xaml_box_impl<T>::boxed_type;

template <typename T>
xaml_result xaml_box_value(T const& value, xaml_object** ptr) noexcept
{
    xaml_ptr<__xaml_boxed_t<T>> obj;
    XAML_RETURN_IF_FAILED(__xaml_box_impl<T>{}(value, &obj));
    return obj->query(ptr);
}

template <typename T>
xaml_result xaml_box_value_s(T const& value, __xaml_boxed_t<T>** ptr) noexcept
{
    return __xaml_box_impl<T>{}(value, ptr);
}

template <typename T>
xaml_ptr<__xaml_boxed_t<T>> xaml_box_value(T const& value)
{
    xaml_ptr<__xaml_boxed_t<T>> obj;
    XAML_THROW_IF_FAILED(__xaml_box_impl<T>{}(value, &obj));
    return obj;
}

template <typename T>
xaml_result xaml_unbox_value(xaml_ptr<xaml_object> const& obj, T* value) noexcept
{
    return __xaml_unbox_impl<T>{}(obj, value);
}

template <typename T>
T xaml_unbox_value(xaml_ptr<xaml_object> const& obj)
{
    T value;
    XAML_THROW_IF_FAILED(xaml_unbox_value(obj, &value));
    return value;
}
#endif // __cplusplus

#endif // !XAML_BOX_H
