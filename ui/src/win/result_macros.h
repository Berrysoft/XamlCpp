#ifndef XAML_UI_NATIVE_WINDOWS_RESULT_MACROS_H
#define XAML_UI_NATIVE_WINDOWS_RESULT_MACROS_H

#include <Windows.h>

#define RETURN_IF_WIN32_BOOL_FALSE(expr) \
    {                                    \
        BOOL __bool_res = (expr);        \
        if (!__bool_res)                 \
        {                                \
            return FALSE;                \
        }                                \
    }

#define RETURN_IF_FAILED(expr) \
    {                          \
        HRESULT __hr = (expr); \
        if (FAILED(__hr))      \
        {                      \
            return __hr;       \
        }                      \
    }

#endif // !XAML_UI_NATIVE_WINDOWS_RESULT_MACROS_H
