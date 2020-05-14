#ifndef XAML_RESULT_POSIX_H
#define XAML_RESULT_POSIX_H

#ifdef __cplusplus
#include <cerrno>
#else
#include <errno.h>
#endif // __cplusplus

#include <xaml/result.h>
#include <xaml/utility.h>

XAML_CONSTEXPR xaml_result xaml_result_from_errno(int err) XAML_NOEXCEPT { return (xaml_result)(err) <= 0 ? (xaml_result)(err) : (xaml_result)((err & 0x0000FFFF) | 0x80000000 | 0x20000000); }

#define XAML_RETURN_IF_POSIX_ERROR(expr)                                       \
    do                                                                         \
    {                                                                          \
        int res = (expr);                                                      \
        if (res == -1)                                                         \
        {                                                                      \
            xaml_result hr = xaml_result_from_errno(errno);                    \
            xaml_result_raise(hr, __XAML_FILE__, __LINE__, __XAML_FUNCTION__); \
            return hr;                                                         \
        }                                                                      \
    } while (0)

#define XAML_GOTO_IF_POSIX_ERROR(expr, label)                                  \
    do                                                                         \
    {                                                                          \
        int res = (expr);                                                      \
        if (res != -1)                                                         \
            hr = XAML_S_OK;                                                    \
        else                                                                   \
        {                                                                      \
            hr = xaml_result_from_errno(errno);                                \
            xaml_result_raise(hr, __XAML_FILE__, __LINE__, __XAML_FUNCTION__); \
            goto label;                                                        \
        }                                                                      \
    } while (0)

#define XAML_ASSERT_POSIX_SUCCESS(expr) \
    do                                  \
    {                                   \
        int res = (expr);               \
        assert(res != -1);              \
    } while (0)

#ifdef __cplusplus
#define XAML_THROW_IF_POSIX_ERROR(expr)                               \
    do                                                                \
    {                                                                 \
        int res = (expr);                                             \
        if (res == -1)                                                \
            throw xaml_result_error{ xaml_result_from_errno(errno) }; \
    } while (0)
#else
#define XAML_THROW_IF_POSIX_ERROR(expr) XAML_ASSERT_POSIX_SUCCESS(expr)
#endif // __cplusplus

#endif // !XAML_RESULT_POSIX_H
