#ifndef XAML_RESULT_H
#define XAML_RESULT_H

#ifdef __cplusplus
#include <cassert>
#include <cstdint>
#include <exception>
#include <stdexcept>
#else
#include <assert.h>
#include <stdint.h>
#endif // __cplusplus

#include <xaml/utility.h>

#if defined(XAML_WIN32) && !defined(XAML_MINGW)
typedef long xaml_result;
#else
typedef XAML_STD int32_t xaml_result;
#endif // XAML_WIN32 && !XAML_MINGW

#define XAML_SUCCEEDED(expr) (!(expr))
#define XAML_FAILED(expr) (expr)

EXTERN_C XAML_API void XAML_CALL xaml_result_raise(xaml_result, xaml_char_t const*, XAML_STD int32_t, xaml_char_t const*) XAML_NOEXCEPT;

#ifdef UNICODE
#define __XAML_FILE__ __FILEW__
#define __XAML_FUNCTION__ __FUNCTIONW__
#else
#define __XAML_FILE__ __FILE__
#define __XAML_FUNCTION__ __FUNCTION__
#endif // UNICODE

#ifdef NDEBUG
#define XAML_RAISE(...)
#else
#define XAML_RAISE(...) xaml_result_raise(__VA_ARGS__)
#endif // NDEBUG

#define XAML_RETURN_IF_FAILED(expr)                                     \
    do                                                                  \
    {                                                                   \
        xaml_result hr = (expr);                                        \
        if (XAML_FAILED(hr))                                            \
        {                                                               \
            XAML_RAISE(hr, __XAML_FILE__, __LINE__, __XAML_FUNCTION__); \
            return hr;                                                  \
        }                                                               \
    } while (0)

#define XAML_GOTO_IF_FAILED(expr, label)                                \
    do                                                                  \
    {                                                                   \
        hr = (expr);                                                    \
        if (XAML_FAILED(hr))                                            \
        {                                                               \
            XAML_RAISE(hr, __XAML_FILE__, __LINE__, __XAML_FUNCTION__); \
            goto label;                                                 \
        }                                                               \
    } while (0)

#define XAML_ASSERT_SUCCEEDED(expr) \
    do                              \
    {                               \
        xaml_result hr = (expr);    \
        assert(XAML_SUCCEEDED(hr)); \
    } while (0)

#define XAML_S_OK 0

#define XAML_E_FAIL 0x80004005
#define XAML_E_INVALIDARG 0x80070057
#define XAML_E_NOINTERFACE 0x80004002
#define XAML_E_NOTIMPL 0x80004001
#define XAML_E_OUTOFMEMORY 0x8007000E
#define XAML_E_KEYNOTFOUND 0x80131577
#define XAML_E_OUTOFBOUNDS 0x80028CA1

#ifdef __cplusplus
struct xaml_result_error : std::exception
{
private:
    xaml_result m_result;

public:
    xaml_result_error(xaml_result result) noexcept : m_result(result) {}

    constexpr xaml_result get_result() const noexcept { return m_result; }
};

#define XAML_THROW_IF_FAILED(expr)         \
    do                                     \
    {                                      \
        xaml_result hr = (expr);           \
        if (XAML_FAILED(hr))               \
            throw xaml_result_error{ hr }; \
    } while (0)

#define XAML_CATCH_RETURN()                                            \
    catch (xaml_result_error const& e) { return e.get_result(); }      \
    catch (std::bad_alloc const&) { return XAML_E_OUTOFMEMORY; }       \
    catch (std::out_of_range const&) { return XAML_E_OUTOFBOUNDS; }    \
    catch (std::invalid_argument const&) { return XAML_E_INVALIDARG; } \
    catch (...) { return XAML_E_FAIL; }
#else
#define XAML_THROW_IF_FAILED(expr) XAML_ASSERT_SUCCEEDED(expr)
#endif // __cplusplus

#endif // !XAML_RESULT_H
