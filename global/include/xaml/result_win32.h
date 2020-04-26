#ifndef XAML_RESULT_WIN32_H
#define XAML_RESULT_WIN32_H

#include <Windows.h>
#include <xaml/result.h>

#define XAML_RETURN_IF_WIN32_BOOL_FALSE(expr)          \
    do                                                 \
    {                                                  \
        BOOL res = (expr);                             \
        if (!res)                                      \
            return HRESULT_FROM_WIN32(GetLastError()); \
    } while (0)

#define XAML_ASSERT_WIN32_BOOL(expr) \
    do                               \
    {                                \
        BOOL res = (expr);           \
        assert(res);                 \
    } while (0)

#ifdef __cplusplus
#define XAML_THROW_IF_WIN32_BOOL_FALSE(expr)                               \
    do                                                                     \
    {                                                                      \
        BOOL res = (expr);                                                 \
        if (!res)                                                          \
            throw xaml_result_error{ HRESULT_FROM_WIN32(GetLastError()) }; \
    } while (0)
#else
#define XAML_THROW_IF_WIN32_BOOL_FALSE(expr) XAML_ASSERT_WIN32_BOOL(expr)
#endif // __cplusplus

#endif // !XAML_RESULT_WIN32_H