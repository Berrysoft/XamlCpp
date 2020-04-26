#ifndef XAML_OBJECT_HPP
#define XAML_OBJECT_HPP

#ifdef __cplusplus
#include <atomic>
#include <cstddef>
#else
#include <stddef.h>
#endif // __cplusplus

#include <xaml/guid.h>
#include <xaml/result.h>
#include <xaml/utility.h>

#ifdef __cplusplus
#define XAML_DECL_INTERFACE(name) struct XAML_NOVTBL name
#define XAML_DECL_INTERFACE_(name, base) struct XAML_NOVTBL name : base
#define XAML_ARGS(type, ...) (__VA_ARGS__)
#define XAML_METHOD_(type, name, ...) virtual type XAML_CALL name XAML_ARGS(__VA_ARGS__) noexcept = 0
#define XAML_METHOD(name, ...) XAML_METHOD_(xaml_result, name, __VA_ARGS__)
#define XAML_VTBL_INHERIT(x)
#define XAML_VTBL_BEGIN
#define XAML_VTBL_END
#else
#define XAML_DECL_INTERFACE(name) struct name
#define XAML_DECL_INTERFACE_(name, base) struct name
#define XAML_ARGS(type, ...) (type* const, ##__VA_ARGS__)
#define XAML_METHOD_(type, name, ...) type(XAML_CALL* name) XAML_ARGS(__VA_ARGS__)
#define XAML_METHOD(name, ...) XAML_METHOD_(xaml_result, name, __VA_ARGS__)
#define XAML_VTBL_INHERIT(x) x
#define XAML_VTBL_BEGIN \
    struct              \
    {
#define XAML_VTBL_END \
    }                 \
    const* vtbl
#endif // __cplusplus

#define XAML_DECL_VTBL(type, vtbl) \
    XAML_VTBL_BEGIN                \
    vtbl(type);                    \
    XAML_VTBL_END

XAML_CLASS(xaml_object, { 0xaf86e2e0, 0xb12d, 0x4c6a, { 0x9c, 0x5a, 0xd7, 0xaa, 0x65, 0x10, 0x1e, 0x90 } })

#define XAML_OBJECT_VTBL(type)                     \
    XAML_METHOD_(XAML_CSTD int32_t, add_ref, type); \
    XAML_METHOD_(XAML_CSTD int32_t, release, type); \
    XAML_METHOD(query, type, xaml_guid XAML_CONST_REF, void**)

XAML_DECL_INTERFACE(xaml_object)
{
    XAML_DECL_VTBL(xaml_object, XAML_OBJECT_VTBL);

#ifdef __cplusplus
    template <typename T>
    xaml_result query(T * *ptr) noexcept
    {
        return query(xaml_type_guid_v<T>, (void**)ptr);
    }
#endif // __cplusplus
};

#ifdef __cplusplus
template <typename T, typename D, typename... Base>
struct xaml_implement : D
{
protected:
    std::atomic<std::int32_t> m_ref_count{ 1 };

public:
    std::int32_t XAML_CALL add_ref() noexcept override { return ++m_ref_count; }

    std::int32_t XAML_CALL release() noexcept override
    {
        std::int32_t res = --m_ref_count;
        if (!res)
        {
            delete static_cast<T*>(this);
            return 0;
        }
        else
        {
            return res;
        }
    }

    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override;
};

template <typename... B>
struct __query_impl;

template <typename B1, typename... B>
struct __query_impl<B1, B...>
{
    template <typename T>
    xaml_result operator()(T* self, xaml_guid const& type, void** ptr) const noexcept
    {
        if (type == xaml_type_guid_v<B1>)
        {
            self->add_ref();
            *ptr = static_cast<B1*>(self);
            return XAML_S_OK;
        }
        else
        {
            return __query_impl<B...>{}(self, type, ptr);
        }
    }
};

template <>
struct __query_impl<>
{
    template <typename T>
    xaml_result operator()(T*, xaml_guid const&, void**) const noexcept
    {
        return XAML_E_NOINTERFACE;
    }
};

template <typename T, typename D, typename... Base>
inline xaml_result xaml_implement<T, D, Base...>::query(xaml_guid const& type, void** ptr) noexcept
{
    return __query_impl<D, Base...>{}(static_cast<T*>(this), type, ptr);
}

template <typename D, typename T, typename... Args>
inline xaml_result xaml_object_new(T** ptr, Args&&... args) noexcept
{
    D* res = new (std::nothrow) D(std::forward<Args>(args)...);
    if (!res) return XAML_E_OUTOFMEMORY;
    *ptr = res;
    return XAML_S_OK;
}

template <typename D, typename T, typename... Args>
inline xaml_result xaml_object_init(T** ptr, Args&&... args) noexcept
{
    D* res = new (std::nothrow) D;
    if (!res) return XAML_E_OUTOFMEMORY;
    xaml_result hr = res->init(std::forward<Args>(args)...);
    if (XAML_SUCCEEDED(hr))
    {
        *ptr = res;
        return XAML_S_OK;
    }
    else
    {
        delete res;
        return hr;
    }
}

template <typename D, typename T, typename... Args>
inline xaml_result xaml_object_new_and_init(T** ptr, Args&&... args) noexcept
{
    D* res = new (std::nothrow) D(std::forward<Args>(args)...);
    if (!res) return XAML_E_OUTOFMEMORY;
    xaml_result hr = res->init();
    if (XAML_SUCCEEDED(hr))
    {
        *ptr = res;
        return XAML_S_OK;
    }
    else
    {
        delete res;
        return hr;
    }
}

template <typename T, typename D, typename Base>
struct xaml_inner_implement : Base
{
    D* m_outer;

    std::int32_t XAML_CALL add_ref() noexcept override { return m_outer->add_ref(); }
    std::int32_t XAML_CALL release() noexcept override { return m_outer->release(); }
    xaml_result XAML_CALL query(xaml_guid const& type, void** ptr) noexcept override { return m_outer->query(type, ptr); }
};
#endif // __cplusplus

#endif // !XAML_OBJECT_HPP
