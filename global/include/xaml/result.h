#ifndef XAML_RESULT_H
#define XAML_RESULT_H

#ifdef __cplusplus
#include <cassert>
#include <cstdint>
#include <exception>
#else
#include <assert.h>
#include <stdint.h>
#endif // __cplusplus

#include <xaml/utility.h>

typedef XAML_CSTD uint32_t xaml_result;

#define XAML_SUCCESS(expr) (!(expr))
#define XAML_FAILED(expr) (expr)

#define XAML_RETURN_IF_FAILED(expr)     \
    do                                  \
    {                                   \
        xaml_result hr = (expr);        \
        if (XAML_FAILED(hr)) return hr; \
    } while (0)

#define XAML_ASSERT_SUCCESS(expr) \
    do                            \
    {                             \
        xaml_result hr = (expr);  \
        assert(XAML_SUCCESS(hr)); \
    } while (0)

#define XAML_S_OK 0

#define XAML_E_FAIL 0x80004005
#define XAML_E_INVALIDARG 0x80070057
#define XAML_E_NOINTERFACE 0x80004002
#define XAML_E_NOTIMPL 0x80004001
#define XAML_E_OUTOFMEMORY 0x8007000E

#ifdef __cplusplus
struct xaml_result_error : std::exception
{
private:
    xaml_result m_result;

public:
    xaml_result_error(xaml_result result) noexcept : exception() {}
    ~xaml_result_error() override {}

    constexpr xaml_result get_result() const noexcept { return m_result; }
};

#define XAML_THROW_IF_FAILED(expr)         \
    do                                     \
    {                                      \
        xaml_result hr = (expr);           \
        if (XAML_FAILED(hr))               \
            throw xaml_result_error{ hr }; \
    } while (0)
#else
#define XAML_THROW_IF_FAILED(expr) XAML_ASSERT_SUCCESS(expr)
#endif // __cplusplus

#endif // !XAML_RESULT_H
