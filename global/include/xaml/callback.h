#ifndef XAML_CALLBACK_H
#define XAML_CALLBACK_H

#ifdef __cplusplus
#include <functional>
#include <xaml/box.h>
#endif // __cplusplus

#include <xaml/object.h>

XAML_CLASS(xaml_callback, { 0x76347aae, 0xb599, 0x4c2e, { 0xa2, 0x02, 0x18, 0x33, 0xca, 0x41, 0xfb, 0x26 } })

#ifdef __cplusplus
struct xaml_callback : xaml_object
{
    virtual xaml_result XAML_CALL invoke(xaml_object*, xaml_object*) const noexcept = 0;
};
#else
#define XAML_CALLBACK_VTBL(type) \
    xaml_result(XAML_CALL* invoke)(type const* const, xaml_object*, xaml_object*);

struct xaml_callback
{
    struct
    {
        XAML_OBJECT_VTBL(xaml_callback)
        XAML_CALLBACK_VTBL(xaml_callback)
    } const* vtbl;
};
#endif // __cplusplus

EXTERN_C XAML_API xaml_result xaml_callback_new(xaml_result (*)(xaml_object*, xaml_object*), xaml_callback**) XAML_NOEXCEPT;

#ifdef __cplusplus
template <typename T>
struct __xaml_callback_impl;

template <typename T1, typename T2>
struct __xaml_callback_impl<void(T1, T2)> : xaml_implement<__xaml_callback_impl<void(T1, T2)>, xaml_callback, xaml_object>
{
private:
    std::function<void(T1, T2)> m_func;

public:
    __xaml_callback_impl(std::function<void(T1, T2)>&& func) noexcept : m_func(std::move(func)) {}

    xaml_result XAML_CALL invoke(xaml_object* sender, xaml_object* args) const noexcept override
    {
        try
        {
            T1 arg1;
            XAML_RETURN_IF_FAILED(unbox_value(sender, arg1));
            T2 arg2;
            XAML_RETURN_IF_FAILED(unbox_value(args, arg2));
            m_func(arg1, arg2);
            return XAML_S_OK;
        }
        catch (xaml_result_error const& e)
        {
            return e.get_result();
        }
        catch (...)
        {
            return XAML_E_FAIL;
        }
    }
};

template <typename T>
inline xaml_result xaml_callback_new(std::function<T>&& func, xaml_callback** ptr) noexcept
{
    if (!func) return XAML_E_INVALIDARG;
    return xaml_object_new<__xaml_callback_impl<T>>(ptr, std::move(func));
}
#endif // __cplusplus

#endif // !XAML_CALLBACK_H
