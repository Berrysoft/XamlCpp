#ifndef XAML_META_METHOD_INFO_H
#define XAML_META_METHOD_INFO_H

#include <xaml/box.h>
#include <xaml/object.h>
#include <xaml/vector.h>

#ifndef xaml_enumerator_1__xaml_object_defined
    #define xaml_enumerator_1__xaml_object_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_O(xaml_object))
#endif // !xaml_enumerator_1__xaml_object_defined

#ifndef xaml_vector_view_1__xaml_object_defined
    #define xaml_vector_view_1__xaml_object_defined
XAML_VECTOR_VIEW_1_TYPE(XAML_T_O(xaml_object))
#endif // !xaml_vector_view_1__xaml_object_defined

#ifndef xaml_enumerator_1__xaml_guid_defined
    #define xaml_enumerator_1__xaml_guid_defined
XAML_ENUMERATOR_1_TYPE(XAML_T_V(xaml_guid))
#endif // !xaml_enumerator_1__xaml_guid_defined

#ifndef xaml_vector_view_1__xaml_guid_defined
    #define xaml_vector_view_1__xaml_guid_defined
XAML_VECTOR_VIEW_1_TYPE(XAML_T_V(xaml_guid))
#endif // !xaml_vector_view_1__xaml_guid_defined

XAML_CLASS(xaml_method_info, { 0xac6df520, 0x582e, 0x46e3, { 0xbd, 0x8b, 0xfb, 0x30, 0x34, 0x3e, 0x16, 0xa1 } })

#define XAML_METHOD_INFO_VTBL(type)                                                  \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type));                                       \
    XAML_METHOD(invoke, type, XAML_VECTOR_VIEW_1_NAME(xaml_object)*, xaml_object**); \
    XAML_METHOD(get_name, type, xaml_string**);                                      \
    XAML_METHOD(get_param_types, type, XAML_VECTOR_VIEW_1_NAME(xaml_guid)**)

XAML_DECL_INTERFACE_(xaml_method_info, xaml_object)
{
    XAML_DECL_VTBL(xaml_method_info, XAML_METHOD_INFO_VTBL);
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_method_info_new(xaml_string*, xaml_result(XAML_CALL*)(XAML_VECTOR_VIEW_1_NAME(xaml_object) *) XAML_NOEXCEPT, XAML_VECTOR_VIEW_1_NAME(xaml_guid) *, xaml_method_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result XAML_CALL xaml_method_info_new(xaml_string*, std::function<xaml_result(xaml_vector_view<xaml_object>*)>&&, xaml_vector_view<xaml_guid>*, xaml_method_info**) noexcept;

template <typename Return, typename... Args, typename F>
Return XAML_CALL __xaml_delegate_noexcept_invoke(F&& f, xaml_result* pres, Args&&... args) noexcept
{
    Return result;
    xaml_result hr = f(std::forward<Args>(args)..., result);
    if (XAML_FAILED(hr)) *pres = hr;
    return result;
}

template <typename... Args, std::int32_t... Indicies>
xaml_result XAML_CALL __xaml_delegate_noexcept_impl_invoke_void_impl(std::function<xaml_result(Args...)> const& func, xaml_vector_view<xaml_object>* args, std::integer_sequence<std::int32_t, Indicies...>) noexcept
{
    xaml_result __hr = XAML_S_OK;
    XAML_RETURN_IF_FAILED(func(__xaml_delegate_noexcept_invoke<Args>(
        [args](int32_t i, auto& arg) noexcept -> xaml_result {
            xaml_ptr<xaml_object> item;
            XAML_RETURN_IF_FAILED(args->get_at(i, &item));
            return xaml_unbox_value(item, &arg);
        },
        &__hr, Indicies)...));
    return __hr;
}

template <typename... Args>
xaml_result XAML_CALL __xaml_delegate_noexcept_impl_invoke_void(std::function<xaml_result(Args...)> const& func, xaml_vector_view<xaml_object>* args)
{
    return __xaml_delegate_noexcept_impl_invoke_void_impl<Args...>(func, args, std::make_integer_sequence<std::int32_t, sizeof...(Args)>{});
}

template <typename... Args>
inline xaml_result XAML_CALL xaml_method_info_new(xaml_string* name, std::function<xaml_result(xaml_interface_t<Args>...)> func, xaml_method_info** ptr) noexcept
{
    if (!func) return XAML_E_INVALIDARG;
    xaml_ptr<xaml_vector<xaml_guid>> param_types;
    XAML_RETURN_IF_FAILED(xaml_vector_new<xaml_guid>({ xaml_type_guid_v<Args>... }, &param_types));
    return xaml_method_info_new(
        name,
        std::function<xaml_result(xaml_vector_view<xaml_object>*)>{
            [func](xaml_vector_view<xaml_object>* args) noexcept -> xaml_result {
                std::int32_t size;
                XAML_RETURN_IF_FAILED(args->get_size(&size));
                if (size < sizeof...(Args)) return XAML_E_INVALIDARG;
                return __xaml_delegate_noexcept_impl_invoke_void<xaml_interface_t<Args>...>(
                    std::function<xaml_result(xaml_interface_t<Args>...)>{
                        [func](xaml_interface_t<Args>... args) noexcept -> xaml_result {
                            return func(std::forward<xaml_interface_t<Args>>(args)...);
                        } },
                    args);
            } },
        param_types,
        ptr);
}

template <typename...>
struct __xaml_delegate_pack_args_impl;

template <typename Arg1, typename... Args>
struct __xaml_delegate_pack_args_impl<Arg1, Args...>
{
    xaml_result operator()(xaml_ptr<xaml_vector<xaml_object>> const& res, Arg1&& arg1, Args&&... args) const noexcept
    {
        xaml_ptr<xaml_object> obj;
        XAML_RETURN_IF_FAILED(xaml_box_value(std::forward<Arg1>(arg1), &obj));
        XAML_RETURN_IF_FAILED(res->append(obj));
        return __xaml_delegate_pack_args_impl<Args...>{}(res, std::forward<Args>(args)...);
    }
};

template <>
struct __xaml_delegate_pack_args_impl<>
{
    xaml_result operator()(xaml_ptr<xaml_vector<xaml_object>> const&) const noexcept
    {
        return XAML_S_OK;
    }
};

template <typename... Args>
xaml_result XAML_CALL xaml_delegate_pack_args(xaml_vector_view<xaml_object>** ptr, Args&&... args) noexcept
{
    xaml_ptr<xaml_vector<xaml_object>> res;
    XAML_RETURN_IF_FAILED(xaml_vector_new(&res));
    XAML_RETURN_IF_FAILED(__xaml_delegate_pack_args_impl<Args...>{}(res, std::forward<Args>(args)...));
    return res->query(ptr);
}
#endif // __cplusplus

XAML_CLASS(xaml_constructor_info, { 0x467e2274, 0x5fc5, 0x4d57, { 0x99, 0x5c, 0x38, 0x3e, 0x60, 0xaa, 0x1e, 0x93 } })

#define XAML_CONSTRUCTOR_INFO_VTBL(type)       \
    XAML_VTBL_INHERIT(XAML_OBJECT_VTBL(type)); \
    XAML_METHOD(invoke, type, xaml_object**);  \
    XAML_METHOD(get_type, type, xaml_guid*)

XAML_DECL_INTERFACE_(xaml_constructor_info, xaml_object)
{
    XAML_DECL_VTBL(xaml_constructor_info, XAML_CONSTRUCTOR_INFO_VTBL);
};

EXTERN_C XAML_META_API xaml_result XAML_CALL xaml_constructor_info_new(xaml_guid XAML_CONST_REF, xaml_result(XAML_CALL*)(xaml_object**), xaml_constructor_info**) XAML_NOEXCEPT;

#ifdef __cplusplus
XAML_META_API xaml_result XAML_CALL xaml_constructor_info_new(xaml_guid const&, std::function<xaml_result(xaml_object**)>&&, xaml_constructor_info**) noexcept;

template <typename T>
xaml_result XAML_CALL xaml_constructor_info_new(xaml_result(XAML_CALL* ctor)(T**), xaml_constructor_info** ptr) noexcept
{
    return xaml_constructor_info_new(
        xaml_type_guid_v<T>,
        std::function<xaml_result(xaml_object**)>{
            [ctor](xaml_object** ptr) -> xaml_result {
                xaml_ptr<T> instance;
                XAML_RETURN_IF_FAILED(ctor(&instance));
                return instance.query(ptr);
            } },
        ptr);
}
#endif // __cplusplus

#endif // !XAML_META_METHOD_INFO_H
