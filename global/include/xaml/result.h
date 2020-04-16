#ifndef XAML_RESULT_H
#define XAML_RESULT_H

#include <assert.h>
#include <stdint.h>

typedef uint32_t xaml_result;

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
#define XAML_E_OUTOFMEMORY 0x8007000E

#endif // !XAML_RESULT_H
